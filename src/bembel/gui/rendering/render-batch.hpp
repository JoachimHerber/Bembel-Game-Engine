#ifndef BEMBEL_GUI_RENDERING_RENDERINGBATCH_HPP
#define BEMBEL_GUI_RENDERING_RENDERINGBATCH_HPP

#include <bembel/kernel/open-gl.hpp>
#include <bembel/kernel/rendering/font.hpp>
#include <bembel/kernel/rendering/text-layout.hpp>

#include "../interaction/interaction-handle.hpp"
#include "style.hpp"

namespace bembel::gui {

enum InstanceType : uint8_t { RECTANGLE = 0, GLYPH = 1, ICON = 3 };

class BEMBEL_API RenderBatchInterface {
  public:
    virtual ~RenderBatchInterface() = default;

    virtual void setColor(const glm::tvec4<uint8_t>& color) = 0;

    virtual void drawRectangle(glm::vec2 min, glm::vec2 max)                                                = 0;
    virtual void drawIcon(glm::vec2 min, glm::vec2 max, glm::vec2 tex_coords_min, glm::vec2 tex_coords_max) = 0;
    virtual void drawGlyph(
        const kernel::Font::Glyph& glyph,
        const glm::vec2&           pos,
        float                      scale,
        float                      threshold_min = 0.45f,
        float                      threshold_max = 0.55f) = 0;
};

struct InstanceData {
    float position_min_x;  // 4 Byte
    float position_min_y;  // 4 Byte
    float position_size_x; // 4 Byte
    float position_size_y; // 4 Byte
    // 16 Byte
    uint16_t tex_coords_min_x; // 2 Byte
    uint16_t tex_coords_min_y; // 2 Byte
    uint16_t tex_coords_max_x; // 2 Byte
    uint16_t tex_coords_max_y; // 2 Byte
    // 24 Byte
    uint8_t color_r; // 1 Byte
    uint8_t color_g; // 1 Byte
    uint8_t color_b; // 1 Byte
    uint8_t alpha;   // 1 Byte
    // 28 Byte
    uint8_t data1; // 1 Byte
    uint8_t data2; // 1 Byte
    uint8_t data3; // 1 Byte
    uint8_t type;  // 1 Byte
    // 32 Byte
};

class BEMBEL_API RenderBatch : public RenderBatchInterface {
  public:
    RenderBatch();
    RenderBatch(const RenderBatch&) = delete;
    ~RenderBatch();

    void init();

    void setPositionOffset(const glm::vec2& position_offset);
    void setDrawArea(const glm::vec2& min, const glm::vec2& max);

    virtual void setColor(const glm::tvec4<uint8_t>& color) override;

    void drawRectangle(glm::vec2 min, glm::vec2 max) override;
    void drawIcon(glm::vec2 min, glm::vec2 max, glm::vec2 tex_coords_min, glm::vec2 tex_coords_max) override;
    void drawGlyph(
        const kernel::Font::Glyph& glyph,
        const glm::vec2&           pos,
        float                      scale,
        float                      threshold_min,
        float                      threshold_max) override;

    void draw();

  private:
    bool clampToViewArea(glm::vec2& min, glm::vec2& max, glm::vec2& tex_coords_min, glm::vec2& tex_coords_max);

    inline void darwElement(
        const glm::vec2& pos_min,
        const glm::vec2& pos_max,
        const glm::vec2& tc_min,
        const glm::vec2& tc_max,
        uint8_t          type,
        uint8_t          data1 = 0u,
        uint8_t          data2 = 0u,
        uint8_t          data3 = 0u) {
        this->instances.push_back(
            {pos_min.x,
             pos_min.y,
             pos_max.x - pos_min.x,
             pos_max.y - pos_min.y,
             uint16_t(0xFFFF * tc_min.x),
             uint16_t(0xFFFF * tc_min.y),
             uint16_t(0xFFFF * (tc_max.x - tc_min.x)),
             uint16_t(0xFFFF * (tc_max.y - tc_min.y)),
             this->color.r,
             this->color.g,
             this->color.b,
             this->color.a,
             data1,
             data2,
             data3,
             type});
    }

  private:
    std::vector<InstanceData> instances;

    glm::tvec4<uint8_t> color;

    glm::vec2 position_offset;
    glm::vec2 draw_area_min;
    glm::vec2 draw_area_max;

    std::size_t buffer_size = 0;

    GLuint vao = 0;
    GLuint vbo = 0;
};

} // namespace bembel::gui
#endif // include guards
