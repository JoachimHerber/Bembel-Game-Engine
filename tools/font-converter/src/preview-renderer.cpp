module;
#include <glbinding/gl/gl.h>

#include "bembel/pch.h"
module bembel.tools.font_converter;

namespace bembel::tools {
using namespace bembel::base;
using namespace bembel::kernel;
using namespace bembel::gui;
using namespace ::gl;

PreviewRenderer::PreviewRenderer(FontFamily* font, Texture* texture)
  : m_font(font)
  , m_texture(texture) {
    m_text.push_back('T');
    m_text.push_back('e');
    m_text.push_back('s');
    m_text.push_back('t');
}

PreviewRenderer::~PreviewRenderer() {}

void PreviewRenderer::draw(glm::ivec2 const& viewport_position, uvec2 const& viewport_size) {
    glClearColor(0.9f, 0.9f, 0.9f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glDisable(GL_CULL_FACE);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 1, 0, 1, -1, 1);
    glTranslatef(0.5f, 0.25f, 0.0f);
    glScalef(0.25f, 0.5f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glBegin(GL_LINES);
    glColor3f(0, 0, 1);
    glVertex3f(-2.f, 0.f, 0.f);
    glVertex3f(+2.f, 0.f, 0.f);
    glEnd();

    m_texture->bind();

    glEnable(GL_TEXTURE_2D);
    drawGlypes();
    glDisable(GL_TEXTURE_2D);
}

void PreviewRenderer::drawGlypes() {
    if(!m_font->getTextureAtlas().getRoot()) return;

    double scale = 1.0 / m_font->getUnitsPerEM();
    glScalef(scale, scale, scale);

    dvec2 size = dvec2(m_font->getTextureAtlas().getRoot()->getSize());

    float advance = 0;
    for(auto c : m_text) {
        int id = m_font->getGlypheID(c, false, false);
        if(id != -1) { advance += m_font->getGlyphs()[id].getAdvance(); }
    }

    float pos = -0.5f * advance;
    for(auto c : m_text) {
        int id = m_font->getGlypheID(c, false, false);
        if(id != -1) {
            Glyph const& glyph = m_font->getGlyphs()[id];

            ivec2 max = glyph.getExtendsMax();
            ivec2 min = glyph.getExtendsMin();

            vec2 texCoordMin = dvec2(glyph.getTexCoordMin()) / size;
            vec2 texCoordMax = dvec2(glyph.getTexCoordMax()) / size;

            glColor3f(1, 1, 1);
            glBegin(GL_QUADS);
            glTexCoord2f(texCoordMin.x, texCoordMin.y);
            glVertex3f(pos + min.x, min.y, 0.f);
            glTexCoord2f(texCoordMin.x, texCoordMax.y);
            glVertex3f(pos + min.x, max.y, 0.f);
            glTexCoord2f(texCoordMax.x, texCoordMax.y);
            glVertex3f(pos + max.x, max.y, 0.f);
            glTexCoord2f(texCoordMax.x, texCoordMin.y);
            glVertex3f(pos + max.x, min.y, 0.f);
            glEnd();

            pos += glyph.getAdvance();
        }
    }
}

void PreviewRenderer::handleEvent(TextInputEvent& event) {
    m_text.push_back(event.character);
    if(m_text.size() > 7) m_text.erase(m_text.begin());
}

} // namespace bembel::tools