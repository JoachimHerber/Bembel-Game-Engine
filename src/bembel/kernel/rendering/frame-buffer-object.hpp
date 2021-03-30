#ifndef BEMBEL_KERNEL_RENDERING_FRAMEBUFFEROBJECT_HPP
#define BEMBEL_KERNEL_RENDERING_FRAMEBUFFEROBJECT_HPP

#include <bembel/kernel/open-gl.hpp>

namespace bembel::kernel {

class Texture;

class BEMBEL_API FrameBufferObject {
  public:
    FrameBufferObject();
    ~FrameBufferObject();

    void init();
    void cleanup();

    void removeAllAttechments();
    void setDepthAttechment(Texture* texture, GLint level = 0);
    void setColorAttechment(unsigned index, Texture* texture, GLint level = 0);

    void beginRenderToTexture();
    void endRenderToTexture();

    void blitToBackBuffer(
        const glm::ivec2& source_min,
        const glm::ivec2& source_max,
        const glm::ivec2& target_min,
        const glm::ivec2& target_max);

  private:
    GLuint handle;

    struct Attechment {
        Texture* texture;
        GLint level;
    };

    Attechment depth_attechment;
    std::vector<Attechment> color_attechments;
};

} // namespace bembel::kernel
#endif // include guards
