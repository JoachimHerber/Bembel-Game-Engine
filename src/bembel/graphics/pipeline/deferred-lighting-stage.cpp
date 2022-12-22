﻿module;
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
    auto& asset_mgr = m_pipline.getAssetManager();

    Asset<ShaderProgram> dir_light_shader;
    Asset<ShaderProgram> point_light_shader;
    dir_light_shader.request(asset_mgr, properties->FirstChildElement("DirectionalLightProgram"));
    point_light_shader.request(asset_mgr, properties->FirstChildElement("PointLightShaderProgram"));

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

void DeferredLightingStage::execute(GeometryRenderQueue&, std::vector<RendererPtr> const&) {
    if(!m_scene) return;

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

void DeferredLightingStage::applyDirectionalLights() {
    ComponentMask mask = m_dir_lights->getComponentMask();

    auto shader = m_dir_light_shader.getAsset();
    if(!shader) return;

    shader->use();
    auto const camera = m_pipline.getCamera();

    mat4 inv_projection = camera->getInverseProjectionMatrix();

    glUniformMatrix4fv(
        shader->getUniformLocation("uInverseProjectionMatrix"), 1, GL_FALSE, &inv_projection[0][0]
    );

    mat3 normal_matrix = camera->getViewMatrix();

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

        if((m_scene->getEntitys()[u64(it.first)] & mask) != mask)
            continue; // this should not happen

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
    auto shader = m_point_light_shader.getAsset();
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
