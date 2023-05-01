module;
#include <optional>
export module bembel.kernel.rendering:FBO;

import bembel.base;
import :Texture;

namespace bembel::kernel {
using namespace bembel::base;

export class FrameBufferObject {
  public:
    FrameBufferObject();
    FrameBufferObject(FrameBufferObject&&)      = default;
    FrameBufferObject(FrameBufferObject const&) = delete;
    ~FrameBufferObject();

    FrameBufferObject& operator=(FrameBufferObject&&)      = default;
    FrameBufferObject& operator=(FrameBufferObject const&) = delete;

    void init();
    void cleanup();

    void removeAllAttechments();
    void setDepthAttechment(Texture* texture, int level = 0, std::optional<int> layer = {});
    void setColorAttechment(
        uint index, Texture* texture, int level = 0, std::optional<int> layer = {}
    );

    void beginRenderToTexture();
    void endRenderToTexture();

    void blitToBackBuffer(
        In<ivec2> source_min, In<ivec2> source_max, In<ivec2> target_min, In<ivec2> target_max
    );

  private:
    uint m_handle = 0;

    struct Attechment {
        Texture*           texture;
        int                level;
        std::optional<int> layer;
    };

    Attechment              m_depth_attechment;
    std::vector<Attechment> m_color_attechments;
};

} // namespace bembel::kernel
