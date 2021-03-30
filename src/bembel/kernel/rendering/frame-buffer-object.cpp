#include "./frame-buffer-object.hpp"

#include "./texture.hpp"

namespace bembel::kernel {

FrameBufferObject::FrameBufferObject()
: handle(0)
, depth_attechment{nullptr, 0} {
}

FrameBufferObject::~FrameBufferObject() {
    cleanup();
}

void FrameBufferObject::init() {
    glGenFramebuffers(1, &(this->handle));
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->handle);
    if(this->depth_attechment.texture) {
        glFramebufferTexture2D(
            GL_DRAW_FRAMEBUFFER,
            GL_DEPTH_ATTACHMENT,
            this->depth_attechment.texture->getTextureTarget(),
            this->depth_attechment.texture->getTextureHandle(),
            this->depth_attechment.level);
    }
    for(unsigned n = 0; n < this->color_attechments.size(); ++n) {
        if(this->color_attechments[n].texture) {
            glFramebufferTexture2D(
                GL_DRAW_FRAMEBUFFER,
                GL_COLOR_ATTACHMENT0 + n,
                this->color_attechments[n].texture->getTextureTarget(),
                this->color_attechments[n].texture->getTextureHandle(),
                this->color_attechments[n].level);
        }
    }

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

void FrameBufferObject::cleanup() {
    if(this->handle != 0) {
        glDeleteFramebuffers(1, &(this->handle));
        this->handle = 0;
    }
}

void FrameBufferObject::removeAllAttechments() {
    setDepthAttechment(nullptr);
    for(unsigned n = 0; n < this->color_attechments.size(); ++n) setColorAttechment(n, nullptr);
}

void FrameBufferObject::setDepthAttechment(Texture* texture, GLint level /*= 0*/) {
    this->depth_attechment.texture = texture;
    this->depth_attechment.level   = level;

    if(this->handle != 0) {
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->handle);
        glFramebufferTexture2D(
            GL_DRAW_FRAMEBUFFER,
            GL_DEPTH_ATTACHMENT,
            texture->getTextureTarget(),
            texture->getTextureHandle(),
            level);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->handle);
    }
}

void FrameBufferObject::setColorAttechment(unsigned index, Texture* texture, GLint level /*= 0*/) {
    while(this->color_attechments.size() <= index)
        this->color_attechments.push_back(Attechment{nullptr, 0});

    this->color_attechments[index].texture = texture;
    this->color_attechments[index].level   = level;

    if(this->handle != 0) {
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->handle);
        glFramebufferTexture2D(
            GL_DRAW_FRAMEBUFFER,
            GL_COLOR_ATTACHMENT0 + index,
            texture->getTextureTarget(),
            texture->getTextureHandle(),
            level);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->handle);
    }
}

void FrameBufferObject::beginRenderToTexture() {
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->handle);

    GLenum status;
    status = glCheckFramebufferStatusEXT(GL_DRAW_FRAMEBUFFER);

    std::vector<GLenum> draw_buffers;
    for(unsigned n = 0; n < this->color_attechments.size(); ++n) {
        if(this->color_attechments[n].texture) draw_buffers.push_back(GL_COLOR_ATTACHMENT0 + n);
    }

    glDrawBuffers(GLsizei(draw_buffers.size()), &draw_buffers[0]);
}

void FrameBufferObject::endRenderToTexture() {
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);
}

void FrameBufferObject::blitToBackBuffer(
    const glm::ivec2& source_min,
    const glm::ivec2& source_max,
    const glm::ivec2& target_min,
    const glm::ivec2& target_max) {
    glBindFramebufferEXT(GL_READ_FRAMEBUFFER, this->handle);
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
        GL_LINEAR);
    glBindFramebufferEXT(GL_READ_FRAMEBUFFER, 0);
}

} // namespace bembel::kernel
