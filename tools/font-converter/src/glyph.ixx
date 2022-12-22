module;
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

    void init(FT_Face& face, uint, uint);

    struct Line {
        dvec2 start;
        dvec2 end;
    };

    std::vector<Line> const&  getOutline() const { return m_outline; }
    std::vector<ivec2> const& getEdges() const { return m_edges; }

    ivec2 const& getExtendsMin() const { return m_extends_min; }
    ivec2 const& getExtendsMax() const { return m_extends_max; }

    ivec2 const& getSize() const { return m_size; }

    void         setTexCoordMin(ivec2 const& value) { m_tex_corrds_min = value; }
    void         setTexCoordMax(ivec2 const& value) { m_tex_corrds_max = value; }
    ivec2 const& getTexCoordMin() const { return m_tex_corrds_min; }
    ivec2 const& getTexCoordMax() const { return m_tex_corrds_max; }

    float getAdvance() const { return m_advance; }

    uint getID() const { return m_id; }
    void setID(uint id) { m_id = id; };

  private:
    struct OutlinePoint {
        ivec2 pos;
        bool  isControlPoint;
        bool  isSecondOrderBézier;
    };

    void readContour(int count, OutlinePoint* points);

    void addBézier(int count, OutlinePoint* points, int start, int end);

    void addBézier(vec2 const&, vec2 const&, vec2 const&);
    void addBézier(vec2 const&, vec2 const&, vec2 const&, vec2 const&);

  private:
    std::vector<Line>  m_outline;
    std::vector<ivec2> m_edges;

    ivec2 m_extends_min = {0, 0};
    ivec2 m_extends_max = {0, 0};

    ivec2 m_size = {0, 0};

    ivec2 m_tex_corrds_min = {0, 0};
    ivec2 m_tex_corrds_max = {0, 0};

    uint m_advance = 0;

    uint m_id = 0;
};

} // namespace bembel::tools
