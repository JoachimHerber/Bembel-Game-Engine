module;
#include <glbinding/gl/gl.h>

#include <utility>
module bembel.graphics.pipeline;

import bembel.base;
import bembel.kernel;
import bembel.graphics.geometry;

namespace bembel::graphics {
using namespace bembel::base;
using namespace bembel::kernel;
using namespace ::gl;

std::vector<std::array<vec4, 8>> getFrustumCornersWorldSpace(
    In<Camera> camera, std::vector<float> cascade_distances
) {
    const auto invers_view = camera.getInverseViewMatrix();
    const auto proj        = camera.getProjectionMatrix();

    float tanHalfVFOV = /****/ 1.f / proj[0][0];
    float tanHalfHFOV = /****/ 1.f / proj[1][1];
    float near        = proj[3][2] / proj[2][2];

    cascade_distances.insert(cascade_distances.begin(), near);

    std::vector<std::array<vec4, 8>> frustum_corners;
    for(usize n = 0; n + 1 < cascade_distances.size(); ++n) {
        float xn = cascade_distances[n + 0] * tanHalfHFOV;
        float xf = cascade_distances[n + 1] * tanHalfHFOV;
        float yn = cascade_distances[n + 0] * tanHalfVFOV;
        float yf = cascade_distances[n + 1] * tanHalfVFOV;

        frustum_corners.push_back(
            {invers_view * vec4(+xn, +yn, -cascade_distances[n + 0], 1.0f),
             invers_view * vec4(-xn, +yn, -cascade_distances[n + 0], 1.0f),
             invers_view * vec4(+xn, -yn, -cascade_distances[n + 0], 1.0f),
             invers_view * vec4(-xn, -yn, -cascade_distances[n + 0], 1.0f),
             invers_view * vec4(+xf, +yf, -cascade_distances[n + 1], 1.0f),
             invers_view * vec4(-xf, +yf, -cascade_distances[n + 1], 1.0f),
             invers_view * vec4(+xf, -yf, -cascade_distances[n + 1], 1.0f),
             invers_view * vec4(-xf, -yf, -cascade_distances[n + 1], 1.0f)}
        );
    }
    return frustum_corners;
}

mat4 calculateLightSpaceTransform(
    In<Camera> camera, In<vec3> light_dir, In<std::array<vec4, 8>> frustum_corners
) {
    vec3 frustum_center = vec3(0, 0, 0);
    for(auto pt : frustum_corners) frustum_center += vec3(pt);
    frustum_center /= frustum_corners.size();

    const auto light_view = glm::lookAt(
        frustum_center - light_dir,
        frustum_center,
        abs(light_dir.y) < 0.9f ? vec3(0.0f, 1.0f, 0.0f) : vec3(1.0f, 0.0f, 0.0f)
    );

    vec3 min = vec3{std::numeric_limits<float>::max()};
    vec3 max = vec3{std::numeric_limits<float>::lowest()};

    for(const auto& v : frustum_corners) {
        const auto trf = light_view * v;
        min            = glm::min(min, vec3(trf));
        max            = glm::max(max, vec3(trf));
    }
    // "pulling back" the near plane to include geometry in front of the frustum of the camera
    max.z += (max.z - min.z) * 10.0f;

    const mat4 light_projection = glm::ortho(min.x, max.x, min.y, max.y, min.z, max.z);
    return light_projection * light_view;
}

void DeferredLightingStage::initRenderQueueForShadowPass() {
    auto const& entities            = m_scene->getEntitys();
    auto*       geometry_container  = m_scene->getComponentContainer<Geometry>();
    auto        transform_container = m_scene->getComponentContainer<Transform>();
    auto        transform           = transform_container->begin();
    auto const& geometrys           = geometry_container->getComponentData();

    m_render_queue.clearRendarData();
    for(usize entity = 0; entity < entities.size(); ++entity, ++transform) {
        if((entities[entity] & geometry_container->getComponentMask()) == 0) continue;

        auto& geom = geometrys[entity];
        if(!geom.cast_shadow) continue;
        mat4 model_matrix = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
        if(entities[entity] & transform_container->getComponentMask()) {
            model_matrix = glm::translate(model_matrix, transform->position);
            model_matrix = model_matrix * mat4_cast(transform->rotation);
            model_matrix = glm::scale(model_matrix, vec3(transform->scale));
        }
        model_matrix = glm::scale(model_matrix, geom.scale);

        m_render_queue.addGeometryObject(geom.model.get(), model_matrix);
    }
    m_render_queue.sortRenderData();
}

void DeferredLightingStage::updateShadowMaps(
    In<std::span<const RendererPtr>>         renderer,
    In<std::span<const std::array<vec4, 8>>> cascadeds_frustum_corners
) {
    auto const camera     = m_pipline.getCamera().get();
    auto       dir_lights = m_scene->getComponentContainer<DirectionalLight>();
    auto       transforms = m_scene->getComponentContainer<Transform>();

    initRenderQueueForShadowPass();

    usize layer = 0;
    for(auto const& it : dir_lights->getComponentData()) {
        if(!it.second.cast_shadow) continue;

        for(auto& frustum_corners : cascadeds_frustum_corners) {
            m_dir_lights.shadows.updateLayer(
                m_render_queue,
                renderer,
                calculateLightSpaceTransform(*camera, it.second.direction, frustum_corners),
                layer++
            );
        }
    }

    glViewport(0, 0, m_pipline.getResulution().x, m_pipline.getResulution().y);
}

void DeferredLightingStage::applyDirectionalLights(In<std::span<const RendererPtr>> renderer) {
    auto          dir_lights = m_scene->getComponentContainer<DirectionalLight>();
    ComponentMask mask       = dir_lights->getComponentMask();

    auto shader = m_dir_lights.shader.get();
    if(!shader) return;

    uint num_shadow_casting_lights = 0;
    for(auto const& it : dir_lights->getComponentData()) {
        if(it.second.cast_shadow) ++num_shadow_casting_lights;
    }
    usize const num_cascadeds = std::min<usize>(
        3, ShadowMap::MAX_NUM_LAYERS / std::max<usize>(num_shadow_casting_lights, 1)
    );

    auto const camera = m_pipline.getCamera().get();
    auto const frustum_corners =
        getFrustumCornersWorldSpace(*camera, m_dir_lights.cascade_distances);

    updateShadowMaps(renderer, frustum_corners);

    beginRenderToTexture();

    shader->use();

    shader->setUniform("uInverseProjectionMatrix", camera->getInverseProjectionMatrix());
    shader->setUniform("uInverseViewMatrix", camera->getInverseViewMatrix());
    shader->setUniform("uShadowMatrices", m_dir_lights.shadows.getTransforms());
    shader->setUniform("uNumCascades", int(m_dir_lights.cascade_distances.size() - 1));
    shader->setUniform("uCascadeDistances", m_dir_lights.cascade_distances);

    glActiveTexture(GL_TEXTURE4);
    m_dir_lights.shadows.getTexture().bind();

    mat3 normal_matrix = camera->getViewMatrix();

    usize layer = 0;
    for(auto const& it : dir_lights->getComponentData()) {
        vec3 dir = it.second.direction;
        dir      = normal_matrix * dir;

        glUniform3f(
            shader->getUniformLocation("uLigthColor"),
            it.second.color.r,
            it.second.color.g,
            it.second.color.b
        );
        glUniform3f(shader->getUniformLocation("uLigthDir"), dir.x, dir.y, dir.z);

        shader->setUniform("uHasShadow", it.second.cast_shadow ? int(1) : int(0));
        if(it.second.cast_shadow) {
            shader->setUniform("uMinShadowMapLayer", int(layer));
            layer += num_cascadeds;
        }

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }

    glUseProgram(0);
}

} // namespace bembel::graphics
