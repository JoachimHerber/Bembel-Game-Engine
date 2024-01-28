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

ShadowMap::ShadowMap(Texture::Target target)
  : m_texture{target, Texture::Format::DEPTH_COMPONENT32} {}
ShadowMap::~ShadowMap() {}

void ShadowMap::setResolution(uint resolution) {
    m_resolution = resolution;

    m_texture.init(
        uvec3(resolution, resolution, MAX_NUM_LAYERS),
        Texture::MinFilter::NEAREST,
        Texture::MagFilter::NEAREST,
        Texture::Wrap::REPEAT,
        Texture::Wrap::REPEAT
    );

    for(uint i = 0; i < MAX_NUM_LAYERS; ++i) {
        m_fbos[i].setDepthAttechment(&m_texture, 0, i);
        m_fbos[i].init();
    }
}

void ShadowMap::updateLayer(
    In<GeometryRenderQueue>          render_queue,
    In<std::span<const RendererPtr>> renderer,
    In<mat4>                         view_proj,
    usize                            layer
) {
    if(layer >= MAX_NUM_LAYERS) return;

    m_transforms[layer] = view_proj;

    m_fbos[layer].beginRenderToTexture();
    glClear(GL_DEPTH_BUFFER_BIT);

    glViewport(0, 0, m_resolution, m_resolution);
    for(auto& it : renderer) {
        if(it) it->renderShadows(view_proj, render_queue.getRenderData());
    }

    m_fbos[layer].endRenderToTexture();
}

} // namespace bembel::graphics
