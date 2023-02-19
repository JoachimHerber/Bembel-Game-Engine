module;
#include <glbinding/gl/gl.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <utility>
module bembel.graphics.pipeline;

import bembel.base;
import bembel.kernel;
import bembel.graphics.geometry;

namespace bembel::graphics {
using namespace bembel::base;
using namespace bembel::kernel;
using namespace ::gl;

DeferredLightingStage::DeferredLightingStage(RenderingPipeline& pipline)
  : RenderingPipeline::Stage(pipline) {}

DeferredLightingStage::~DeferredLightingStage() {}

void DeferredLightingStage::setDirLightShader(Asset<ShaderProgram> shader) {
    m_dir_light_shader = std::move(shader);
}
void DeferredLightingStage::setPointLightShader(Asset<ShaderProgram> shader) {
    m_point_light_shader = std::move(shader);
}

bool DeferredLightingStage::configure(xml::Element const* properties) {
    Asset<ShaderProgram> dir_light_shader;
    Asset<ShaderProgram> point_light_shader;
    dir_light_shader.request(properties->FirstChildElement("DirectionalLightProgram"));
    point_light_shader.request(properties->FirstChildElement("PointLightShaderProgram"));

    setDirLightShader(std::move(dir_light_shader));
    setPointLightShader(std::move(point_light_shader));

    std::string texture_name;
    if(xml::getAttribute(properties, "Output", "texture", texture_name))
        setColorOutputTexture(0, texture_name);

    std::vector<std::string> intput_textures;
    for(auto input : xml::IterateChildElements(properties, "Input")) {
        if(base::xml::getAttribute(input, "texture", texture_name))
            intput_textures.push_back(texture_name);
    }
    setInputTextures(intput_textures);

    return true;
}

void DeferredLightingStage::setScene(Scene* scene) {
    m_scene = scene;
    if(scene) {
        m_scene->registerComponentType<DirectionalLight>();
        m_scene->registerComponentType<PointLight>();
        m_scene->registerComponentType<Transform>();

        m_dir_lights   = m_scene->getComponentContainer<DirectionalLight>();
        m_point_lights = m_scene->getComponentContainer<PointLight>();
        m_transforms   = m_scene->getComponentContainer<Transform>();

    } else {
        m_dir_lights   = nullptr;
        m_point_lights = nullptr;
        m_transforms   = nullptr;
    }
}

inline void setVertexAttribPointer(uint index, int size, uint offset) {
    glVertexAttribPointer(
        index, size, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(offset * sizeof(float))
    );
}

void DeferredLightingStage::init() {
    RenderingPipeline::Stage::init();

    m_buffer_size = 32;
    glGenBuffers(1, &(m_vbo));
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, m_buffer_size * 8 * sizeof(float), nullptr, GL_STREAM_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenVertexArrays(1, &(m_vao));
    glBindVertexArray(m_vao);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    setVertexAttribPointer(0, 3, 0);         // Position
    setVertexAttribPointer(1, 1, 3);         // BulbRadius
    setVertexAttribPointer(2, 3, 3 + 1);     // Color
    setVertexAttribPointer(3, 1, 3 + 1 + 3); // CutoffRadius
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glVertexAttribDivisor(0, 1);
    glVertexAttribDivisor(1, 1);
    glVertexAttribDivisor(2, 1);
    glVertexAttribDivisor(3, 1);

    glBindVertexArray(0);
}

void DeferredLightingStage::cleanup() {
    RenderingPipeline::Stage::cleanup();
}

void DeferredLightingStage::execute(
    GeometryRenderQueue& render_queue, std::vector<RendererPtr> const& renderer
) {
    if(!m_scene) return;

    updateShadowMaps(render_queue, renderer);


    m_fbo->beginRenderToTexture();

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    glBlendFunc(GL_ONE, GL_ONE);
    glEnable(GL_BLEND);

    bindInputTextures();

    applyDirectionalLights();
    applyPointLights();

    releaseInputTextures();

    glDisable(GL_BLEND);

    m_fbo->endRenderToTexture();
}

std::array<vec4, 8> getFrustumCornersWorldSpace(Camera const& camera) {
    const auto inv = camera.getInverseViewMatrix()
                   * camera.getInverseProjectionMatrix(); // glm::inverse(proj * view) ;

    std::array<vec4, 8> frustum_corners = {
        vec4{-1.f, -1.f, -1.f, 1.f},
        vec4{-1.f, -1.f, +1.f, 1.f},
        vec4{-1.f, +1.f, -1.f, 1.f},
        vec4{-1.f, +1.f, +1.f, 1.f},
        vec4{+1.f, -1.f, -1.f, 1.f},
        vec4{+1.f, -1.f, +1.f, 1.f},
        vec4{+1.f, +1.f, -1.f, 1.f},
        vec4{+1.f, +1.f, +1.f, 1.f}};

    for(auto& pt : frustum_corners) {
        pt = inv * pt;
        pt /= pt.w;
    }
    return frustum_corners;
}

mat4 calculateLightSpaceTransform(
    In<std::array<vec4, 8>> frustum_corners, In<vec3> frustum_center, In<vec3> light_dir
) {
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

void DeferredLightingStage::initRenderQueueForShadowPass(GeometryRenderQueue& render_queue) {
    auto const& entities           = m_scene->getEntitys();
    auto*       geometry_container = m_scene->getComponentContainer<Geometry>();
    auto const& geometrys          = geometry_container->getComponentData();
    auto        transform          = m_scene->getComponentContainer<Transform>()->begin();

    render_queue.clearRendarData();
    for(usize entity = 0; entity < entities.size(); ++entity, ++transform) {
        if((entities[entity] & geometry_container->getComponentMask()) == 0) continue;

        auto& geom = geometrys[entity];
        if(!geom.cast_shadow) continue;
        mat4 model_matrix = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
        if(entities[entity] & m_transforms->getComponentMask()) {
            model_matrix = glm::translate(model_matrix, transform->position);
            model_matrix = model_matrix * glm::mat4_cast(transform->rotation);
            model_matrix = glm::scale(model_matrix, vec3(transform->scale));
        }
        model_matrix = glm::scale(model_matrix, geom.scale);

        render_queue.addGeometryObject(geom.model.get(), model_matrix);
    }
    render_queue.sortRenderData();
}

void DeferredLightingStage::updateShadowMaps(
    GeometryRenderQueue& render_queue, std::vector<RendererPtr> const& renderer
) {
    ComponentMask mask = m_dir_lights->getComponentMask();

    auto const camera = m_pipline.getCamera().get();

    auto frustum_corners = getFrustumCornersWorldSpace(*camera);
    vec3 frustum_center  = vec3(0, 0, 0);
    for(auto pt : frustum_corners) frustum_center += vec3(pt);
    frustum_center /= frustum_corners.size();

    initRenderQueueForShadowPass(render_queue);

    for(auto const& it : m_dir_lights->getComponentData()) {
        if(!it.second.shadow_map) continue;

        auto shadow_map = it.second.shadow_map.get();

        shadow_map->world_to_light_space =
            calculateLightSpaceTransform(
            frustum_corners, frustum_center, it.second.direction
        );
        mat4 view_proj = shadow_map->world_to_light_space;

        glViewport(0, 0, shadow_map->resolution, shadow_map->resolution);
        shadow_map->fbo.beginRenderToTexture();
        glClear(GL_DEPTH_BUFFER_BIT);

        for(auto& it : renderer) { it->renderShadows(view_proj, render_queue.getRenderData()); }
        shadow_map->fbo.endRenderToTexture();
    }

    glViewport(0, 0, m_pipline.getResulution().x, m_pipline.getResulution().y);
}

void DeferredLightingStage::applyDirectionalLights() {
    ComponentMask mask = m_dir_lights->getComponentMask();

    auto shader = m_dir_light_shader.get();
    if(!shader) return;

    shader->use();
    auto const camera = m_pipline.getCamera();

    mat4 inverse_proj  = camera->getInverseProjectionMatrix();
    mat3 normal_matrix = camera->getViewMatrix();
    mat4 inverse_view  = camera->getInverseViewMatrix();

    shader->setUniform("uInverseProjectionMatrix", inverse_proj);
    shader->setUniform("uInverseViewMatrix", inverse_view);

    for(auto const& it : m_dir_lights->getComponentData()) {
        vec3 dir = it.second.direction;
        dir      = normal_matrix * dir;

        glUniform3f(
            shader->getUniformLocation("uLigthColor"),
            it.second.color.r,
            it.second.color.g,
            it.second.color.b
        );
        glUniform3f(shader->getUniformLocation("uLigthDir"), dir.x, dir.y, dir.z);

        shader->setUniform("uHasShadow", it.second.shadow_map ? int(1) : int(0));
        if(it.second.shadow_map) {
            mat4 shadow_matrix = it.second.shadow_map->world_to_light_space;
            shader->setUniform("uShadowMatrix", shadow_matrix);
            glActiveTexture(GL_TEXTURE4);
            it.second.shadow_map->texture.bind();
        }

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }

    glUseProgram(0);
}

struct PointLightRenderingData {
    float x, y, z;
    float bulb_radius;
    float r, g, b;
    float cutoff_radius;

    PointLightRenderingData(In<vec4> pos, In<PointLightData> light)
      : x{pos.x}
      , y{pos.y}
      , z{pos.z}
      , bulb_radius{light.bulb_radius}
      , r{light.color.r}
      , g{light.color.g}
      , b{light.color.b}
      , cutoff_radius{light.cutoff_radius} {}
};

void DeferredLightingStage::applyPointLights() {
    auto shader = m_point_light_shader.get();
    if(!shader) return;

    ComponentMask mask = m_point_lights->getComponentMask() | m_transforms->getComponentMask();

    Camera* camera = m_pipline.getCamera().get();

    std::vector<PointLightRenderingData> point_lights;
    for(auto const& [entity, point_light] : m_point_lights->getComponentData()) {
        if((m_scene->getEntitys()[std::to_underlying(entity)] & mask) != mask)
            continue; // this should not happen

        vec4 position = vec4(m_transforms->getComponent(entity)->position, 1);
        position      = camera->getViewMatrix() * position;

        point_lights.emplace_back(position, point_light);
    }

    shader->use();

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

    mat4 projection        = camera->getProjectionMatrix();
    mat4 invers_projection = camera->getInverseProjectionMatrix();

    glUniformMatrix4fv(
        shader->getUniformLocation("uProjectionMatrix"), 1, GL_FALSE, &projection[0][0]
    );
    glUniformMatrix4fv(
        shader->getUniformLocation("uInverseProjectionMatrix"),
        1,
        GL_FALSE,
        &invers_projection[0][0]
    );
    // glUniform2f(
    //     shader->getUniformLocation("uTexelSize"),
    //     1.0f / m_pipline.getResulution().x,
    //     1.0f / m_pipline.getResulution().y);

    for(size_t n = 0; n < point_lights.size(); n += m_buffer_size) {
        GLsizei num_lights = std::min(m_buffer_size, uint(point_lights.size() - n));
        glBufferSubData(
            GL_ARRAY_BUFFER, 0, num_lights * sizeof(PointLightRenderingData), &point_lights[n]
        );

        glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, 10, num_lights);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glUseProgram(0);
}

} // namespace bembel::graphics
