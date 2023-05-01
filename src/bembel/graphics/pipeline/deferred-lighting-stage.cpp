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
    m_dir_lights.shader = std::move(shader);
}
void DeferredLightingStage::setPointLightShader(Asset<ShaderProgram> shader) {
    m_point_lights.shader = std::move(shader);
}

void DeferredLightingStage::setDirLightShadowResolution(uint resolution) {
    m_dir_lights.shadows.setResolution(resolution);
}

void DeferredLightingStage::setDirLightShadowCascadeDistances(In<std::span<const float>> cascades) {
    m_dir_lights.cascade_distances.clear();
    m_dir_lights.cascade_distances.insert(
        m_dir_lights.cascade_distances.end(), cascades.begin(), cascades.end()
    );
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
        if(xml::getAttribute(input, "texture", texture_name))
            intput_textures.push_back(texture_name);
    }

    setInputTextures(intput_textures);

    if(auto shadows = properties->FirstChildElement("DirectionalShadows")) {
        std::vector<float> cascade_distances;

        for(auto cascade : xml::IterateChildElements(shadows, "Cascade")) {
            float dist;
            if(xml::getAttribute(cascade, "max_distance", dist)) cascade_distances.push_back(dist);
        }

        uint shadow_resolution = 1024;
        xml::getAttribute(shadows, "resolution", shadow_resolution);
        setDirLightShadowResolution(shadow_resolution);
        setDirLightShadowCascadeDistances(cascade_distances);
    }


    return true;
}

void DeferredLightingStage::setScene(Scene* scene) {
    m_scene = scene;
    if(scene) {
        m_scene->registerComponentType<DirectionalLight>();
        m_scene->registerComponentType<PointLight>();
        m_scene->registerComponentType<Transform>();
    }
    // m_dir_lights   = m_scene->getComponentContainer<DirectionalLight>();
    // m_point_lights = m_scene->getComponentContainer<PointLight>();
    // m_transforms   = m_scene->getComponentContainer<Transform>();
}

inline void setVertexAttribPointer(uint index, int size, uint offset) {
    glVertexAttribPointer(
        index, size, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(offset * sizeof(float))
    );
}

void DeferredLightingStage::init() {
    RenderingPipeline::Stage::init();

    m_point_lights.buffer_size = 32;
    glGenBuffers(1, &(m_point_lights.vbo));
    glBindBuffer(GL_ARRAY_BUFFER, m_point_lights.vbo);
    glBufferData(
        GL_ARRAY_BUFFER, m_point_lights.buffer_size * 8 * sizeof(float), nullptr, GL_STREAM_DRAW
    );
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenVertexArrays(1, &(m_point_lights.vao));
    glBindVertexArray(m_point_lights.vao);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);

    glBindBuffer(GL_ARRAY_BUFFER, m_point_lights.vbo);
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

void DeferredLightingStage::execute(In<std::span<const RendererPtr>> renderer) {
    if(!m_scene) return;

    applyDirectionalLights(renderer);
    applyPointLights(renderer);
}

void DeferredLightingStage::beginRenderToTexture() {
    m_fbo->beginRenderToTexture();

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    glBlendFunc(GL_ONE, GL_ONE);
    glEnable(GL_BLEND);

    bindInputTextures();

    gl::glViewport(0, 0, m_pipline.getResulution().x, m_pipline.getResulution().y);
}

void DeferredLightingStage::endRenderToTexture() {
    releaseInputTextures();

    glDisable(GL_BLEND);

    m_fbo->endRenderToTexture();
}

} // namespace bembel::graphics
