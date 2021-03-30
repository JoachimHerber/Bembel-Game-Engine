#ifndef BEMBEL_GRAPHICS_RENDERINGPIPELINEVIEW_HPP
#define BEMBEL_GRAPHICS_RENDERINGPIPELINEVIEW_HPP

#include <bembel/kernel/assets/asset-manager.hpp>
#include <bembel/kernel/display/viewport.hpp>
#include <bembel/kernel/open-gl.hpp>
#include <bembel/kernel/rendering/frame-buffer-object.hpp>
#include <bembel/kernel/rendering/texture.hpp>

namespace bembel::graphics {

class BEMBEL_API TextureView : public kernel::Viewport::View {
  public:
    TextureView(kernel::Texture* texture);
    ~TextureView();

    void       setViewArea(const glm::ivec2& pos, const glm::uvec2& size);
    glm::ivec2 getViewAreaPosition() const;
    glm::uvec2 getViewAreaSize() const;

    void init();
    void cleanup();

    virtual void draw(
        const glm::ivec2& viewport_position, const glm::uvec2& viewport_size) override;

  private:
    kernel::Texture* texture;

    glm::ivec2 view_area_pos;
    glm::uvec2 view_area_size;

    std::unique_ptr<kernel::FrameBufferObject> fbo;
};

} // namespace bembel::graphics
#endif // include guards
