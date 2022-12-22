module;
export module bembel.kernel.rendering:FBO;

import bembel.base;
import :Texture;

namespace bembel::kernel {
using namespace bembel::base;

export class FrameBufferObject {
  public:
    FrameBufferObject();
    ~FrameBufferObject();

    void init();
    void cleanup();

    void removeAllAttechments();
    void setDepthAttechment(Texture* texture, int level = 0);
    void setColorAttechment(unsigned index, Texture* texture, int level = 0);

    void beginRenderToTexture();
    void endRenderToTexture();

    void blitToBackBuffer(
        In<ivec2> source_min, In<ivec2> source_max, In<ivec2> target_min, In<ivec2> target_max
    );

  private:
    uint m_handle = 0;

    struct Attechment {
        Texture* texture;
        int      level;
    };

    Attechment              m_depth_attechment;
    std::vector<Attechment> m_color_attechments;
};

} // namespace bembel::kernel
