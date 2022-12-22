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

dvec2 toVec2(FT_Vector& point) {
    return {point.x, point.y};
}

Glyph::Glyph() {}
Glyph::~Glyph() {}

void Glyph::init(FT_Face& face, uint glyph_index, uint border) {
    auto error = FT_Load_Glyph(
        face,        /* handle to face object */
        glyph_index, /* glyph index           */
        FT_LOAD_NO_SCALE
    ); /* load flags, see below */
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
        if(0x0001 & outline.tags[i]) {
            m_edges.emplace_back(outline.points[i].x, outline.points[i].y);
        }
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

void Glyph::readContour(int count, OutlinePoint* points) {
    int start = 0;
    while(start < count && points[start].isControlPoint) ++start;

    int prev = start;
    for(int i = 1; i <= count; ++i) {
        int           current = (i + start);
        OutlinePoint* point   = &points[current % count];
        if(point->isControlPoint) continue;
        if(prev + 1 == current) {
            m_outline.emplace_back(points[prev % count].pos, points[current % count].pos);
        } else
            addBézier(count, points, prev, current);
        prev = current;
    }
}

void Glyph::addBézier(int count, OutlinePoint* points, int start, int end) {
    if(points[(start + 1) % count].isSecondOrderBézier) {
        vec2 prev    = points[(start + 0) % count].pos;
        vec2 current = points[(start + 1) % count].pos;
        for(int i = start + 2; i < end; ++i) {
            vec2 next = points[(i) % count].pos;
            addBézier(prev, current, 0.5f * (current + next));
            prev    = 0.5f * (current + next);
            current = next;
        }
        addBézier(prev, current, points[end % count].pos);
    } else {
        if(start + 3 == end) {
            addBézier(
                points[(start + 0) % count].pos,
                points[(start + 1) % count].pos,
                points[(start + 2) % count].pos,
                points[(start + 3) % count].pos
            );
        }
    }
}
void Glyph::addBézier(vec2 const& start, vec2 const& control, vec2 const& end) {
    vec2 prev = start;
    for(int i = 0; i < 16; ++i) {
        double f = (i + 1.0f) / (16.0f);
        double g = 1 - f;

        vec2 point;

        point.x = g * g * start.x + 2 * g * f * control.x + f * f * end.x;
        point.y = g * g * start.y + 2 * g * f * control.y + f * f * end.y;

        m_outline.emplace_back(prev, point);

        prev = point;
    }
}
void Glyph::addBézier(
    vec2 const& start, vec2 const& control1, vec2 const& control2, vec2 const& end
) {
    vec2 prev = start;
    for(int i = 0; i < 16; ++i) {
        double f = (i + 1.0f) / (16.0f);
        double g = 1 - f;

        vec2 point;
        point.x = 1 * g * g * g * start.x + 3 * g * g * f * control1.x + 3 * g * f * f * control2.x
                + 1 * f * f * f * end.x;
        point.y = 1 * g * g * g * start.y + 3 * g * g * f * control1.y + 3 * g * f * f * control2.y
                + 1 * f * f * f * end.y;
        m_outline.push_back(Line{prev, point});

        prev = point;
    }
}

} // namespace bembel::tools
