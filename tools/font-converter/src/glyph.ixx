module;
#include <map>
#include <span>
#include <vector>
//
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_IMAGE_H
export module bembel.tools.font_converter:Glyph;

import bembel;

namespace bembel::tools {
using namespace bembel::base;
using namespace bembel::kernel;
using namespace bembel::gui;

export class Glyph {
  public:
    Glyph();
    ~Glyph();

    void init(FT_Face& face, uint, uint, std::map<unsigned, int>& glyph_map, std::vector<Glyph>& glyphs);

    struct SubGlyph {
        int        index;
        dvec2      position;
        dmat2     transform;
    };

    std::span<const std::vector<ivec2>> getOutline()   const { return m_outline; }
    std::span<const SubGlyph>           getSubGlyphs() const { return m_sub_glyphs; }

    ivec2 getExtendsMin() const { return m_extends_min; }
    ivec2 getExtendsMax() const { return m_extends_max; }

    void setExtendsMax(ivec2 max) { m_extends_max = max; }

    ivec2 const& getSize() const { return m_size; }

    void         setTexCoordMin(In<ivec2> value) { m_tex_corrds_min = value; }
    void         setTexCoordMax(In<ivec2> value) { m_tex_corrds_max = value; }
    ivec2 const& getTexCoordMin() const { return m_tex_corrds_min; }
    ivec2 const& getTexCoordMax() const { return m_tex_corrds_max; }

    float getAdvance() const { return m_advance; }

    uint getID() const { return m_id; }
    void setID(uint id) { m_id = id; };

  private:
    struct OutlinePoint {
        ivec2 pos;
        bool  is_control_point;
        bool  is_second_order_bézier;
    };

    void readContour(int count, OutlinePoint* points);

    void addBézier(int count, OutlinePoint* points, int start, int end);

    void addBézier(vec2 const&, vec2 const&, vec2 const&);
    void addBézier(vec2 const&, vec2 const&, vec2 const&, vec2 const&);

  private:
    std::vector <std::vector<ivec2>> m_outline;
    std::vector<SubGlyph>            m_sub_glyphs;

    ivec2 m_extends_min = {0, 0};
    ivec2 m_extends_max = {0, 0};

    ivec2 m_size = {0, 0};

    ivec2 m_tex_corrds_min = {0, 0};
    ivec2 m_tex_corrds_max = {0, 0};

    uint m_advance = 0;

    uint m_id = 0;
};

} // namespace bembel::tools
