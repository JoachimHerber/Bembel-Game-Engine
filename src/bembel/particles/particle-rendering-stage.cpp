module;
#include <glbinding/gl/gl.h>

#include <algorithm>
#include <cstdlib>
module bembel.particles;

import bembel.base;
import bembel.kernel;
import :Data;

namespace bembel::particles {
using namespace bembel::base;
using namespace bembel::kernel;
using namespace gl;

ParticleRenderingStage::ParticleRenderingStage(RenderingPipeline& pipline)
  : RenderingPipeline::Stage(pipline) {}

ParticleRenderingStage::~ParticleRenderingStage() {}

bool ParticleRenderingStage::configure(xml::Element const* properties) {
    kernel::Asset<kernel::ShaderProgram> shader;
    shader.request(properties->FirstChildElement("Shader"));
    if(!shader) return false;

    Asset<Texture> texture;
    texture.request(properties->FirstChildElement("Texture"));
    if(!texture) return false;

    std::string depth_texture, color_texture;
    if(!xml::getAttribute(properties, "Depth", "texture", depth_texture)
       || !xml::getAttribute(properties, "Output", "texture", color_texture)) {
        return true;
    }
    setDepthOutputTexture(depth_texture);
    setColorOutputTexture(0, color_texture);
    m_shader  = std::move(shader);
    m_texture = std::move(texture);
    return true;
}

void ParticleRenderingStage::setScene(Scene* scene) {
    m_scene = scene;
}

void ParticleRenderingStage::init() {
    RenderingPipeline::Stage::init();

    u64 buffer_size = 1024 * 1024;
    m_data.reserve(buffer_size);
    glGenBuffers(1, &(m_vbo));
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(
        GL_ARRAY_BUFFER, buffer_size * sizeof(ParticleRenderData), nullptr, GL_STREAM_DRAW
    );
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenVertexArrays(1, &(m_vao));
    glBindVertexArray(m_vao);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glVertexAttribPointer(
        0, 4, GL_FLOAT, GL_FALSE, sizeof(ParticleRenderData), (void*)(0)
    ); // Position & Size
    glVertexAttribPointer(
        1, 4, GL_UNSIGNED_SHORT, GL_TRUE, sizeof(ParticleRenderData), (void*)(16)
    ); // tex_coordse
    glVertexAttribPointer(
        2, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(ParticleRenderData), (void*)(24)
    ); // color
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glVertexAttribDivisor(0, 1);
    glVertexAttribDivisor(1, 1);
    glVertexAttribDivisor(2, 1);

    glBindVertexArray(0);
}

void ParticleRenderingStage::cleanup() {
    RenderingPipeline::Stage::cleanup();
}

void ParticleRenderingStage::execute(In<std::span<const RendererPtr>> renderer) {
    if(!m_scene) return;

    auto shader = m_shader.get();
    if(!shader) return;
    auto texture = m_texture.get();
    if(!texture) return;

    auto* data = m_scene->getDataContainer<ParticleData>();

    auto camera = m_pipline.getCamera();

    vec3 cam_pos = camera->getPosition();
    vec3 cam_dir = camera->getInverseViewMatrix() * vec4(0, 0, 1, 1);

    m_data.clear();
    for(auto& population : data->getParticlePopulations()) {
        auto type = population.getType();
        for(auto& particel : population.getParticles()) {
            float f = float(particel.age) / float(type->max_age);

            m_data.emplace_back(
                particel.position,
                type->calculateParticleSize(f),
                type->calculateTexturCoords(f),
                particel.color,
                glm::dot(particel.position - cam_pos, cam_dir)
            );
        }
    }
    std::sort(m_data.begin(), m_data.end(), [](In<ParticleRenderData> a, In<ParticleRenderData> b) {
        return a.dist < b.dist;
    });

    m_fbo->beginRenderToTexture();

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

    gl::glEnable(gl::GL_CULL_FACE);
    gl::glEnable(gl::GL_DEPTH_TEST);
    gl::glEnable(gl::GL_BLEND);

    shader->use();
    glActiveTexture(GL_TEXTURE0);
    texture->bind();

    shader->setUniform("uViewMatrix", camera->getViewMatrix());
    shader->setUniform("uProjectionMatrix", camera->getProjectionMatrix());

    glBufferSubData(GL_ARRAY_BUFFER, 0, m_data.size() * sizeof(ParticleRenderData), m_data.data());

    glDepthMask(GL_FALSE);
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, m_data.size());
    glDepthMask(GL_TRUE);

    m_fbo->endRenderToTexture();
}

} // namespace bembel::particles
