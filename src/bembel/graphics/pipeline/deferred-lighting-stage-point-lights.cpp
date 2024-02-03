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

struct PointLightRenderingData {
    float x, y, z;
    float bulb_radius;
    float r, g, b;
    float cutoff_radius;

    PointLightRenderingData(In<vec4> pos, In<PointLight> light)
      : x{pos.x}
      , y{pos.y}
      , z{pos.z}
      , bulb_radius{light.bulb_radius}
      , r{light.color.r}
      , g{light.color.g}
      , b{light.color.b}
      , cutoff_radius{light.cutoff_radius} {}
};

void DeferredLightingStage::applyPointLights(In<std::span<const RendererPtr>>) {
    auto shader = m_point_lights.shader.get();
    if(!shader) return;

    auto point_light_container = m_scene->getComponentContainer<PointLight>();
    auto transforms            = m_scene->getComponentContainer<Transform>();
    if(!point_light_container || !transforms) return;

    ComponentMask mask = point_light_container->getComponentMask() | transforms->getComponentMask();

    Camera* camera = m_pipline.getCamera().get();

    std::vector<PointLightRenderingData> point_lights;
    for(auto const& [entity, point_light] : point_light_container->getComponentData()) {
        if((m_scene->getEntitys()[std::to_underlying(entity)] & mask) != mask)
            continue; // this should not happen

        vec4 position = vec4(transforms->getComponent(entity)->position, 1);
        position      = camera->getViewMatrix() * position;

        point_lights.emplace_back(position, point_light);
    }

    shader->use();

    glBindVertexArray(m_point_lights.vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_point_lights.vbo);

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

    for(size_t n = 0; n < point_lights.size(); n += m_point_lights.buffer_size) {
        GLsizei num_lights = std::min(m_point_lights.buffer_size, uint(point_lights.size() - n));
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
