
#include "./preview-renderer.hpp"

//#include <GLFW/glfw3.h>

#include <iostream>

PreviewRenderer::PreviewRenderer(FontFamily* font, bembel::kernel::Texture* texture)
: font(font)
, texture(texture) {
    this->text.push_back('T');
    this->text.push_back('e');
    this->text.push_back('s');
    this->text.push_back('t');
}

PreviewRenderer::~PreviewRenderer() {
}

void PreviewRenderer::draw(const glm::ivec2& viewport_position, const glm::uvec2& viewport_size) {
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

    this->texture->bind();

    glEnable(GL_TEXTURE_2D);
    this->drawGlypes();
    glDisable(GL_TEXTURE_2D);
}

void PreviewRenderer::drawGlypes() {
    if(!this->font->getTextureAtlas().getRoot()) return;

    double scale = 1.0 / this->font->getUnitsPerEM();
    glScalef(scale, scale, scale);

    glm::dvec2 size = glm::dvec2(this->font->getTextureAtlas().getRoot()->getSize());

    float advance = 0;
    for(auto c : this->text) {
        int id = this->font->getGlypheID(c, false, false);
        if(id != -1) { advance += this->font->getGlyphs()[id].getAdvance(); }
    }

    float pos = -0.5f * advance;
    for(auto c : this->text) {
        int id = this->font->getGlypheID(c, false, false);
        if(id != -1) {
            const Glyph& glyph = this->font->getGlyphs()[id];

            glm::ivec2 max = glyph.getExtendsMax();
            glm::ivec2 min = glyph.getExtendsMin();

            glm::vec2 texCoordMin = glm::dvec2(glyph.getTexCoordMin()) / size;
            glm::vec2 texCoordMax = glm::dvec2(glyph.getTexCoordMax()) / size;

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

void PreviewRenderer::handleEvent(bembel::TextInputEvent& event) {
    this->text.push_back(event.character);
    if(this->text.size() > 7) this->text.erase(this->text.begin());
}
