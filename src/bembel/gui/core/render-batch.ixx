export module bembel.gui.core:RenderBatch;

import bembel.base;
import bembel.kernel;

namespace bembel::gui {
using namespace bembel::base;
using namespace bembel::kernel;

export enum InstanceType : u8 { RECTANGLE = 0, GLYPH = 1, ICON = 2 };

export class RenderBatchInterface {
  public:
    virtual ~RenderBatchInterface() = default;

    virtual void setColor(In<ColorRGBA> color)         = 0;
    virtual void setPrimaryColor(In<ColorRGB> color)   = 0;
    virtual void setSecondaryColor(In<ColorRGB> color) = 0;
    virtual void setAlpha(In<u8> alpha)                = 0;

    virtual void drawRectangle(vec2 min, vec2 max)                                      = 0;
    virtual void drawIcon(vec2 min, vec2 max, vec2 tex_coords_min, vec2 tex_coords_max) = 0;
    virtual void drawGlyph(
        Font::Glyph const& glyph,
        vec2 const&        pos,
        float              scale,
        float              threshold_min = 0.48f,
        float              threshold_max = 0.52f
    ) = 0;
};

export struct InstanceData {
    float position_min_x;  // 4 Byte
    float position_min_y;  // 4 Byte
    float position_size_x; // 4 Byte
    float position_size_y; // 4 Byte
    // 16 Byte
    u16 tex_coords_min_x; // 2 Byte
    u16 tex_coords_min_y; // 2 Byte
    u16 tex_coords_max_x; // 2 Byte
    u16 tex_coords_max_y; // 2 Byte
    // 24 Byte
    u8 primary_color_r; // 1 Byte
    u8 primary_color_g; // 1 Byte
    u8 primary_color_b; // 1 Byte
    u8 alpha;           // 1 Byte
    // 28 Byte
    u8 secondary_color_r; // 1 Byte
    u8 secondary_color_g; // 1 Byte
    u8 secondary_color_b; // 1 Byte
    u8 type;              // 1 Byte
    // 32 Byte
};

export class RenderBatch : public RenderBatchInterface {
  public:
    RenderBatch()                   = default;
    RenderBatch(RenderBatch const&) = delete;
    ~RenderBatch()                  = default;

    void init();

    void setPositionOffset(vec2 const& position_offset);
    void setDrawArea(vec2 const& min, vec2 const& max);

    virtual void setColor(In<ColorRGBA> color) override;
    virtual void setPrimaryColor(In<ColorRGB> color) override;
    virtual void setSecondaryColor(In<ColorRGB> color) override;
    virtual void setAlpha(In<u8> alpha) override;

    virtual void drawRectangle(vec2 min, vec2 max) override;
    virtual void drawIcon(vec2 min, vec2 max, vec2 tex_coords_min, vec2 tex_coords_max) override;
    virtual void drawGlyph(
        Font::Glyph const& glyph,
        vec2 const&        pos,
        float              scale,
        float              threshold_min,
        float              threshold_max
    ) override;

    void draw();

  private:
    bool clampToViewArea(vec2& min, vec2& max, vec2& tex_coords_min, vec2& tex_coords_max);

    void addInstance(
        vec2 pos_min,
        vec2 pos_max,
        vec2 tc_min,
        vec2 tc_max,
        u8   type,
        u8   data1 = 0u,
        u8   data2 = 0u,
        u8   data3 = 0u
    ) {
        m_instances.emplace_back(
            pos_min.x,
            pos_min.y,
            pos_max.x - pos_min.x,
            pos_max.y - pos_min.y,
            u16(0xFFFF * tc_min.x),
            u16(0xFFFF * tc_min.y),
            u16(0xFFFF * (tc_max.x - tc_min.x)),
            u16(0xFFFF * (tc_max.y - tc_min.y)),
            m_primary_color.r,
            m_primary_color.g,
            m_primary_color.b,
            m_alpha,
            data1,
            data2,
            data3,
            type
        );
    }

  private:
    std::vector<InstanceData> m_instances;

    ColorRGB m_primary_color;
    ColorRGB m_secondary_color;
    u8       m_alpha;

    vec2 m_position_offset;
    vec2 m_draw_area_min;
    vec2 m_draw_area_max;

    usize m_buffer_size = 0;

    uint m_vao = 0;
    uint m_vbo = 0;
};

} // namespace bembel::gui
