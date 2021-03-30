#include "./render-batch.hpp"

#include <bembel/base/io/xml.hpp>
#include <bembel/base/logging/logger.hpp>
#include <bembel/kernel/assets/asset-manager.hpp>
#include <bembel/kernel/core/kernel.hpp>
#include <bembel/kernel/display/display-manager.hpp>
#include <bembel/kernel/display/window.hpp>
#include <bembel/kernel/rendering/font.hpp>
#include <bembel/kernel/rendering/shader.hpp>

namespace bembel::gui {

RenderBatch::RenderBatch() {
}

RenderBatch::~RenderBatch() {
}

void setupVertexAttribute(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei offset) {
    glEnableVertexAttribArray(index);
    glVertexAttribPointer(index, size, type, normalized, sizeof(InstanceData), (void*)(offset));
    glVertexAttribDivisor(index, 1);
}

void RenderBatch::init() {
    glGenVertexArrays(1, &(this->vao));
    glBindVertexArray(this->vao);

    glGenBuffers(1, &(this->vbo));
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);

    // Positions           4*sizeof(GL_FLOAT)          = 16 Byte
    setupVertexAttribute(0, 4, GL_FLOAT, GL_FALSE, 0);
    // TexCoords           4*sizeof(GL_UNSIGNED_SHORT) = 8 Byte
    setupVertexAttribute(1, 4, GL_UNSIGNED_SHORT, GL_TRUE, 16);
    //  color              4*sizeof(GL_UNSIGNED_SHORT) = 4 Byte
    setupVertexAttribute(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, 16 + 8);
    // additional data     4*sizeof(GL_UNSIGNED_SHORT) = 4 Byte
    setupVertexAttribute(3, 4, GL_UNSIGNED_BYTE, GL_FALSE, 16 + 8 + 4);
}

void RenderBatch::setPositionOffset(const glm::vec2& position_offset) {
    this->position_offset = position_offset;
}

void RenderBatch::setDrawArea(const glm::vec2& min, const glm::vec2& max) {
    this->draw_area_min = min;
    this->draw_area_max = max;
}

void RenderBatch::setColor(const glm::tvec4<uint8_t>& color) {
    this->color = color;
}

void RenderBatch::draw() {
    if(this->instances.empty()) return;

    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    // resize buffer if nececary
    std::size_t required_buffer_size = sizeof(InstanceData) * this->instances.size();

    if(required_buffer_size > this->buffer_size) {
        this->buffer_size = std::max(2 * this->buffer_size, required_buffer_size * 3 / 2);
        glBufferData(GL_ARRAY_BUFFER, this->buffer_size, nullptr, GL_STREAM_DRAW);
    }
    // upload data
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(InstanceData) * this->instances.size(), &(this->instances[0]));
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // draw instances
    glBindVertexArray(this->vao);
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, this->instances.size());

    // clear data
    this->instances.clear();
}

void RenderBatch::drawRectangle(glm::vec2 min, glm::vec2 max) {
    min += this->position_offset;
    max += this->position_offset;

    min = glm::max(this->draw_area_min, min);
    max = glm::min(this->draw_area_max, max);

    if(max.x <= min.x || max.y <= min.y) {
        return; // outside of draw area
    }
    this->darwElement(min, max, {0.0f, 0.0f}, {1.0f, 1.0f}, InstanceType::RECTANGLE);
}

void RenderBatch::drawIcon(glm::vec2 min, glm::vec2 max, glm::vec2 tex_coords_min, glm::vec2 tex_coords_max) {
    min += this->position_offset;
    max += this->position_offset;

    if(!clampToViewArea(min, max, tex_coords_min, tex_coords_max)) { return; }

    this->darwElement(min, max, tex_coords_min, tex_coords_max, InstanceType::ICON);
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
    const kernel::Font::Glyph& glyph, const glm::vec2& pos, float scale, float threshold_min, float threshold_max) {
    glm::vec2 min    = glyph.extents_min * scale + pos + this->position_offset;
    glm::vec2 max    = glyph.extents_max * scale + pos + this->position_offset;
    glm::vec2 tc_min = glyph.tex_coords_min;
    glm::vec2 tc_max = glyph.tex_coords_max;
    if(!clampToViewArea(min, max, tc_min, tc_max)) return;

    this->darwElement(
        min, max, tc_min, tc_max, InstanceType::GLYPH, uint8_t(threshold_min * 0xff), uint8_t(threshold_max * 0xff));
}

bool RenderBatch::clampToViewArea(
    glm::vec2& min, glm::vec2& max, glm::vec2& tex_coords_min, glm::vec2& tex_coords_max) {
    if(max.x <= this->draw_area_min.x || min.x >= this->draw_area_max.x || max.y <= this->draw_area_min.y
       || min.y >= this->draw_area_max.y) {
        return false; // outside of draw area
    }

    // clamp lower bounds
    auto scale = (tex_coords_max - tex_coords_min) / (max - min);
    if(min.x < this->draw_area_min.x) {
        tex_coords_min.x -= (min.x - this->draw_area_min.x) * scale.x;
        min.x = this->draw_area_min.x;
    }
    if(min.y < this->draw_area_min.y) {
        tex_coords_min.y -= (min.y - this->draw_area_min.y) * scale.y;
        min.y = this->draw_area_min.y;
    }
    // upper
    if(max.x > this->draw_area_max.x) {
        tex_coords_max.x -= (max.x - this->draw_area_max.x) * scale.x;
        max.x = this->draw_area_max.x;
    }
    if(max.y > this->draw_area_max.y) {
        tex_coords_max.y -= (max.y - this->draw_area_max.y) * scale.y;
        max.y = this->draw_area_max.y;
    }
    return true;
}

} // namespace bembel::gui
