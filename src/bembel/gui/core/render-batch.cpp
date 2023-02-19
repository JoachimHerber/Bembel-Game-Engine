module;
#include <glbinding/gl/gl.h>
module bembel.gui.core;

import bembel.base;
import bembel.kernel;

namespace bembel::gui {
using namespace bembel::base;
using namespace bembel::kernel;
using namespace ::gl;

void setupVertexAttribute(
    GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei offset
) {
    glEnableVertexAttribArray(index);
    glVertexAttribPointer(
        index, size, type, normalized, sizeof(InstanceData), (void*)(uintptr_t)offset
    );
    glVertexAttribDivisor(index, 1);
}

void RenderBatch::init() {
    glGenVertexArrays(1, &(m_vao));
    glBindVertexArray(m_vao);

    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

    // Positions           4*sizeof(GL_FLOAT)          = 16 Byte
    setupVertexAttribute(0, 4, GL_FLOAT, GL_FALSE, 0);
    // TexCoords           4*sizeof(GL_UNSIGNED_SHORT) = 8 Byte
    setupVertexAttribute(1, 4, GL_UNSIGNED_SHORT, GL_TRUE, 16);
    //  color              4*sizeof(GL_UNSIGNED_SHORT) = 4 Byte
    setupVertexAttribute(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, 16 + 8);
    // additional data     4*sizeof(GL_UNSIGNED_SHORT) = 4 Byte
    setupVertexAttribute(3, 4, GL_UNSIGNED_BYTE, GL_FALSE, 16 + 8 + 4);
}

void RenderBatch::setPositionOffset(vec2 const& position_offset) {
    m_position_offset = position_offset;
}

void RenderBatch::setDrawArea(vec2 const& min, vec2 const& max) {
    m_draw_area_min = min;
    m_draw_area_max = max;
}

void RenderBatch::setColor(In<ColorRGBA> color) {
    m_primary_color   = color;
    m_secondary_color = color;
    m_alpha           = color.a;
}

void RenderBatch::setPrimaryColor(In<ColorRGB> color) {
    m_primary_color = color;
}
void RenderBatch::setSecondaryColor(In<ColorRGB> color) {
    m_secondary_color = color;
}
void RenderBatch::setAlpha(In<u8> alpha) {
    m_alpha = alpha;
}

void RenderBatch::draw() {
    if(m_instances.empty()) return;

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    // resize buffer if nececary
    usize required_buffer_size = sizeof(InstanceData) * m_instances.size();

    if(required_buffer_size > m_buffer_size) {
        m_buffer_size = std::max(2 * m_buffer_size, required_buffer_size * 3 / 2);
        glBufferData(GL_ARRAY_BUFFER, m_buffer_size, nullptr, GL_STREAM_DRAW);
    }
    // upload data
    glBufferSubData(
        GL_ARRAY_BUFFER, 0, sizeof(InstanceData) * m_instances.size(), &(m_instances[0])
    );
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // draw instances
    glBindVertexArray(m_vao);
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, m_instances.size());
    glBindVertexArray(0);
    // clear data
    m_instances.clear();
}

void RenderBatch::drawRectangle(vec2 min, vec2 max) {
    min += m_position_offset;
    max += m_position_offset;

    min = base::max(m_draw_area_min, min);
    max = base::min(m_draw_area_max, max);

    if(max.x <= min.x || max.y <= min.y) {
        return; // outside of draw area
    }
    addInstance(min, max, {0.0f, 0.0f}, {1.0f, 1.0f}, InstanceType::RECTANGLE);
}

void RenderBatch::drawIcon(
    glm::vec2 min, glm::vec2 max, glm::vec2 tex_coords_min, glm::vec2 tex_coords_max
) {
    min += m_position_offset;
    max += m_position_offset;

    if(!clampToViewArea(min, max, tex_coords_min, tex_coords_max)) { return; }

    addInstance(
        min,
        max,
        tex_coords_min,
        tex_coords_max,
        InstanceType::ICON,
        m_secondary_color.r,
        m_secondary_color.g,
        m_secondary_color.b
    );
}

/*
void RenderBatch::drawText(const kernel::TextLayout& text, glm::vec2 pos, float scale) {
    auto font = text.getFont();
    if(!font) return;

    for(const auto& glyph : text.getGlyphs()) {
        auto      extends    = font->getGlyphExtents(glyph.id);
        auto      tex_coords = font->getGlyphTexCoord(glyph.id);
        glm::vec2 p          = pos + scale * glyph.pos;
        drawGlyph(
            {p.x + scale * extends.x, p.y + scale * extends.z},
            {p.x + scale * extends.y, p.y + scale * extends.w},
            {tex_coords.x, tex_coords.z},
            {tex_coords.y, tex_coords.w});
    }
}
//*/

void RenderBatch::drawGlyph(
    Font::Glyph const& glyph, vec2 const& pos, float scale, float threshold_min, float threshold_max
) {
    vec2 min    = glyph.extents_min * scale + pos + m_position_offset;
    vec2 max    = glyph.extents_max * scale + pos + m_position_offset;
    vec2 tc_min = glyph.tex_coords_min;
    vec2 tc_max = glyph.tex_coords_max;
    if(!clampToViewArea(min, max, tc_min, tc_max)) return;

    addInstance(
        min,
        max,
        tc_min,
        tc_max,
        InstanceType::GLYPH,
        u8(threshold_min * 0xff),
        u8(threshold_max * 0xff)
    );
}

bool RenderBatch::clampToViewArea(
    vec2& min, vec2& max, vec2& tex_coords_min, vec2& tex_coords_max
) {
    if(max.x <= m_draw_area_min.x || min.x >= m_draw_area_max.x || max.y <= m_draw_area_min.y
       || min.y >= m_draw_area_max.y) {
        return false; // outside of draw area
    }

    // clamp lower bounds
    auto scale = (tex_coords_max - tex_coords_min) / (max - min);
    if(min.x < m_draw_area_min.x) {
        tex_coords_min.x -= (min.x - m_draw_area_min.x) * scale.x;
        min.x = m_draw_area_min.x;
    }
    if(min.y < m_draw_area_min.y) {
        tex_coords_min.y -= (min.y - m_draw_area_min.y) * scale.y;
        min.y = m_draw_area_min.y;
    }
    // upper
    if(max.x > m_draw_area_max.x) {
        tex_coords_max.x -= (max.x - m_draw_area_max.x) * scale.x;
        max.x = m_draw_area_max.x;
    }
    if(max.y > m_draw_area_max.y) {
        tex_coords_max.y -= (max.y - m_draw_area_max.y) * scale.y;
        max.y = m_draw_area_max.y;
    }
    return true;
}

} // namespace bembel::gui
