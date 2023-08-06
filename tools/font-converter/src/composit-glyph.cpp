module;
//
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_IMAGE_H
module bembel.tools.font_converter;

import bembel;
import :FontFamily;

namespace bembel::tools {
using namespace bembel::base;
using namespace bembel::kernel;
using namespace bembel::gui;

CompositGlyph::CompositGlyph() {}
CompositGlyph::~CompositGlyph() {}

void CompositGlyph::init(FT_Face& face, uint glyph_index, uint border) {
    auto error = FT_Load_Glyph(
        face,        /* handle to face object */
        glyph_index, /* glyph index           */
        FT_LOAD_NO_SCALE
    );
    if(error) return;

    FT_Outline& outline = face->glyph->outline;

    m_advance = face->glyph->metrics.horiAdvance;

    std::vector<OutlinePoint> points;
    for(int i = 0; i < outline.n_points; ++i) {
        points.emplace_back(
            ivec2(outline.points[i].x, outline.points[i].y),
            (0x0001 & outline.tags[i]) == 0,
            (0x0002 & outline.tags[i]) == 0
        );
        if(0x0001 & outline.tags[i]) { m_edges.emplace_back(outline.points[i].x, outline.points[i].y); }
    }

    short start = 0;
    for(int i = 0; i < outline.n_contours; ++i) {
        short count = outline.contours[i] - start + 1;
        readContour(count, &(points[start]));
        start = outline.contours[i] + 1;
    }

    if(!points.empty()) {
        m_extends_min = m_extends_max = points[0].pos;
        for(size_t n = 1; n < points.size(); ++n) {
            m_extends_min = min(m_extends_min, points[n].pos);
            m_extends_max = max(m_extends_max, points[n].pos);
        }
    }
    //  if (face->glyph->metrics.horiBearingY != _extendsMax.y)
    //  {
    //    cout << "ERROR" << endl;
    //  }

    m_extends_min -= ivec2(border);
    m_extends_max += ivec2(border);

    m_size = m_extends_max - m_extends_min;
}

} // namespace bembel::tools
