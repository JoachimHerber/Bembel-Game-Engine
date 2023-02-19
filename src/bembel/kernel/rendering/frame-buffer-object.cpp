module;
#include <glbinding/gl/gl.h>
module bembel.kernel.rendering;

import bembel.base;

import :FBO;

namespace bembel::kernel {
using namespace bembel::base;
using namespace ::gl;

namespace gl {
    inline void setFramebufferTexture2D(GLenum attachment, Texture* texture, int level) {
        if(texture) {
            glFramebufferTexture2D(
                GL_DRAW_FRAMEBUFFER,
                attachment,
                static_cast<GLenum>(texture->getTextureTarget()),
                texture->getTextureHandle(),
                level
            );
        }
    }
} // namespace gl

FrameBufferObject::FrameBufferObject() : m_depth_attechment{nullptr, 0} {}

FrameBufferObject::~FrameBufferObject() {
    cleanup();
}

void FrameBufferObject::init() {
    glGenFramebuffers(1, &(m_handle));
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_handle);
    gl::setFramebufferTexture2D(
        GL_DEPTH_ATTACHMENT, m_depth_attechment.texture, m_depth_attechment.level
    );
    for(unsigned n = 0; n < m_color_attechments.size(); ++n) {
        gl::setFramebufferTexture2D(
            GL_COLOR_ATTACHMENT0 + n, m_color_attechments[n].texture, m_color_attechments[n].level
        );
    }

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

void FrameBufferObject::cleanup() {
    if(m_handle != 0) {
        glDeleteFramebuffers(1, &m_handle);
        m_handle = 0;
    }
}

void FrameBufferObject::removeAllAttechments() {
    setDepthAttechment(nullptr);
    for(unsigned n = 0; n < m_color_attechments.size(); ++n) setColorAttechment(n, nullptr);
}

void FrameBufferObject::setDepthAttechment(Texture* texture, GLint level /*= 0*/) {
    m_depth_attechment.texture = texture;
    m_depth_attechment.level   = level;

    if(m_handle != 0) {
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_handle);
        gl::setFramebufferTexture2D(GL_DEPTH_ATTACHMENT, texture, level);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    }
}

void FrameBufferObject::setColorAttechment(unsigned index, Texture* texture, GLint level /*= 0*/) {
    while(m_color_attechments.size() <= index)
        m_color_attechments.push_back(Attechment{nullptr, 0});

    m_color_attechments[index].texture = texture;
    m_color_attechments[index].level   = level;

    if(m_handle != 0) {
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_handle);
        gl::setFramebufferTexture2D(GL_COLOR_ATTACHMENT0 + index, texture, level);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    }
}

void FrameBufferObject::beginRenderToTexture() {
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_handle);

    GLenum status;
    status = glCheckFramebufferStatusEXT(GL_DRAW_FRAMEBUFFER);

    std::vector<GLenum> draw_buffers;
    for(unsigned n = 0; n < m_color_attechments.size(); ++n) {
        if(m_color_attechments[n].texture) draw_buffers.push_back(GL_COLOR_ATTACHMENT0 + n);
    }
    if(!draw_buffers.empty()) {
        glDrawBuffers(GLsizei(draw_buffers.size()), &draw_buffers[0]);
    } else {
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
    }
}

void FrameBufferObject::endRenderToTexture() {
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);
}

void FrameBufferObject::blitToBackBuffer(
    ivec2 source_min, ivec2 source_max, ivec2 target_min, ivec2 target_max
) {
    glBindFramebufferEXT(GL_READ_FRAMEBUFFER, m_handle);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

    glBlitFramebufferEXT(
        source_min.x,
        source_min.y,
        source_max.x,
        source_max.y,
        target_min.x,
        target_min.y,
        target_max.x,
        target_max.y,
        GL_COLOR_BUFFER_BIT,
        GL_NEAREST
    );
    glBindFramebufferEXT(GL_READ_FRAMEBUFFER, 0);
}

} // namespace bembel::kernel
