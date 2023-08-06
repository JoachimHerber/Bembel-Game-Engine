module;
#include <glbinding/gl/gl.h>

#include <glm/glm.hpp>
module bembel.tools.font_converter;

import bembel;

namespace bembel::tools {
using namespace bembel::base;
using namespace bembel::kernel;
using namespace bembel::gui;
using namespace ::gl;

FontView::FontView(GlyphTextureAtlas& texture_atlas, FontTextureGenerator& texture_gen)
  : m_texture_atlas{texture_atlas}, m_texture_gen{texture_gen} {}

FontView::~FontView() {}

void FontView::setFont(FontFamily* font) {
    m_font = font;
}

void FontView::draw(ivec2 const& viewport_position, uvec2 const& viewport_size) {
    m_texture_gen.getFBO()->blitToBackBuffer(
        {0, 0}, uvec2{m_texture_gen.getResolution()},
        viewport_position,
        viewport_position + ivec2(viewport_size)
    );

    if(!m_font) return;

    glViewport(viewport_position.x, viewport_position.y, viewport_size.x, viewport_size.y);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.f, 1.f, 0.f, 1.f, -1.f, 1.f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glScalef(1.f / m_texture_gen.getResolution(), 1.f / m_texture_gen.getResolution(), 1.f);
    glDisable(GL_CULL_FACE);
    glDisable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);

    glUseProgram(0);
    glLineWidth(1);
    glBegin(GL_LINES);

    glColor3f(0, 0, 1);

    for(auto const& glyph : m_font->getGlyphs()) {
        auto const& min = glyph.getTexCoordMin();
        auto const& max = glyph.getTexCoordMax();

        glColor3f(0, 0, 1);
        glVertex3f(min.x, min.y, 0.f);
        glVertex3f(min.x, max.y, 0.f);
        glVertex3f(max.x, min.y, 0.f);
        glVertex3f(max.x, max.y, 0.f);

        glVertex3f(min.x, min.y, 0.f);
        glVertex3f(max.x, min.y, 0.f);
        glVertex3f(min.x, max.y, 0.f);
        glVertex3f(max.x, max.y, 0.f);

        vec2 origin = -glyph.getExtendsMin();
        origin /= glyph.getSize();
        origin = glm::clamp(origin, vec2(0), vec2(1));
        origin *= max - min;
        origin += min;

        glColor3f(0, 1, 1);
        glVertex3f(min.x, origin.y, 0.f);
        glVertex3f(max.x, origin.y, 0.f);
        glVertex3f(origin.x, min.y, 0.f);
        glVertex3f(origin.x, max.y, 0.f);
    }
    glEnd();
}

} // namespace bembel::tools
