module;
#include <glbinding/gl/gl.h>

#include "bembel/pch.h"
module bembel.tools.font_converter;

import bembel;

namespace bembel::tools {
using namespace bembel::base;
using namespace bembel::kernel;
using namespace bembel::gui;
using namespace ::gl;

FontView::FontView(AssetManager& asset_mgr) : m_asset_mgr(asset_mgr) {
    initFBO();
    m_shader.request(asset_mgr, "gen_dist_field.glsl");

    //_resolutionChangedSignal.AddSlot(this, &FontRenderer::OnResolutionChanged);
}

FontView::~FontView() {
    if(m_font) {
        auto& atlas = m_font->getTextureAtlas();
        atlas.update_signal.unbind(this, &FontView::onAtlasUpdate);
    }
}

void FontView::setFont(FontFamily* font) {
    if(m_font) {
        auto& atlas = m_font->getTextureAtlas();
        atlas.update_signal.unbind(this, &FontView::onAtlasUpdate);
    }
    m_font = font;

    if(m_font) {
        auto& atlas = m_font->getTextureAtlas();
        atlas.update_signal.bind(this, &FontView::onAtlasUpdate);
        atlas.setResolution(m_resolution);
    }
}

void FontView::update() {
    if(!m_requires_update) return;

    glUseProgram(0);

    float oldViewport[4];
    m_fbo->beginRenderToTexture();

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glDisable(GL_SCISSOR_TEST);
    glViewport(0, 0, m_resolution.x, m_resolution.y);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.f, 1.f, 0.f, 1.f, -1.f, 1.f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glScalef(1.f / m_resolution.x, 1.f / m_resolution.y, 1.f);

    renderToTexture();

    glEnable(GL_SCISSOR_TEST);
    m_fbo->endRenderToTexture();
    glDisable(GL_SCISSOR_TEST);

    m_requires_update = false;
}

void FontView::renderToTexture() {
    if(m_font == nullptr) return;

    glDisable(GL_BLEND);
    glLineWidth(1);
    glColor4f(0, 1, 0, 1);
    glBegin(GL_LINES);

    for(int i = 0; i < m_resolution.y; ++i) {
        float y = (i + 0.5f);

        Intersections intersections;
        getIntersections(y, m_font->getTextureAtlas().getRoot(), intersections);

        std::sort(
            intersections.begin(),
            intersections.end(),
            [](Intersection const& first, Intersection const& second) {
                return first.pos < second.pos;
            }
        );

        float        start = 0.0f;
        int          c     = 0;
        Intersection last  = {-1, false};
        for(auto& it : intersections) {
            if(abs(last.pos - it.pos) < 0.01 && last.direction == it.direction)
                continue; // ignore duplicates.

            if(c == 0) start = it.pos;

            c += it.direction ? 1 : -1;

            if(c == 0) {
                glVertex3f(start, y, 0);
                glVertex3f(it.pos, y, 0);
            }
            last = it;
        }
    }
    glEnd();

    glEnable(GL_BLEND);
    glBlendEquation(GL_MAX);

    m_shader->use();
    drawGlypeOutline(m_font->getTextureAtlas().getRoot());
    glUseProgram(0);

    glBlendEquation(GL_FUNC_ADD);
}

void FontView::draw(ivec2 const& viewport_position, uvec2 const& viewport_size) {
    if(!m_font) return;

    update();

    m_fbo->blitToBackBuffer(
        {0, 0}, m_resolution, viewport_position, viewport_position + ivec2(viewport_size)
    );

    glViewport(viewport_position.x, viewport_position.y, viewport_size.x, viewport_size.y);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.f, 1.f, 0.f, 1.f, -1.f, 1.f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glScalef(1.f / m_resolution.x, 1.f / m_resolution.y, 1.f);

    glUseProgram(0);
    glLineWidth(1);
    glBegin(GL_LINES);
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

void FontView::initFBO() {
    m_texture = std::make_unique<Texture>(Texture::Target::TEXTURE_2D);
    m_texture->init(m_resolution);

    m_fbo = std::make_unique<FrameBufferObject>();
    m_fbo->setColorAttechment(0, m_texture.get());
    m_fbo->init();
}

void FontView::getIntersections(
    double y, TextureAtlasNode const* node, Intersections& intersections
) {
    if(!node) return; // node dos not exist
    if(node->getPos().y > y || node->getPos().y + node->getSize().y < y)
        return; // the y-position is outside of the node area

    getIntersections(y, node->getFirstChildNode(), intersections);
    getIntersections(y, node->getSecondChildNode(), intersections);

    auto glyph = node->getGlyph();
    if(!glyph) return;

    y -= node->getPos().y;
    y /= node->getSize().y;
    y *= glyph->getSize().y;
    y += glyph->getExtendsMin().y;

    for(auto& line : glyph->getOutline()) {
        if(line.end.y < y && line.start.y < y) continue;
        if(line.end.y > y && line.start.y > y) continue;

        vec2 dir = line.end - line.start;

        if(dir.y == 0) continue;

        double dist = length(dir);
        dir /= dist;

        double f = (y - line.start.y) / dir.y;
        double x = f * dir.x + line.start.x;
        x -= glyph->getExtendsMin().x;
        x /= glyph->getSize().x;
        x *= node->getSize().x;
        x += node->getPos().x;

        intersections.push_back({x, dir.y < 0 ? true : false});
    }
}

void FontView::drawGlypeOutline(TextureAtlasNode const* node) {
    if(!node) return;

    drawGlypeOutline(node->getFirstChildNode());
    drawGlypeOutline(node->getSecondChildNode());

    auto glyph = node->getGlyph();
    if(!glyph) return;

    for(auto& it : glyph->getOutline()) {
        dvec2 start = it.start;
        dvec2 end   = it.end;

        start -= dvec2(glyph->getExtendsMin());
        end -= dvec2(glyph->getExtendsMin());
        start /= dvec2(glyph->getSize());
        end /= dvec2(glyph->getSize());
        start *= dvec2(glyph->getTexCoordMax() - glyph->getTexCoordMin());
        end *= dvec2(glyph->getTexCoordMax() - glyph->getTexCoordMin());
        start += dvec2(glyph->getTexCoordMin());
        end += dvec2(glyph->getTexCoordMin());

        dvec2 X = m_line_width * glm::normalize(start - end);
        dvec2 Y = {X.y, -X.x};

        glBegin(GL_TRIANGLE_STRIP);
        glTexCoord2f(+1.f, +1.f);
        glVertex3f(start.x + X.x + Y.x, start.y + X.y + Y.y, 0.f);
        glTexCoord2f(+1.f, -1.f);
        glVertex3f(start.x + X.x - Y.x, start.y + X.y - Y.y, 0.f);

        glTexCoord2f(-0.f, +1.f);
        glVertex3f(start.x + Y.x, start.y + Y.y, 0.f);
        glTexCoord2f(-0.f, -1.f);
        glVertex3f(start.x - Y.x, start.y - Y.y, 0.f);

        glTexCoord2f(-0.f, +1.f);
        glVertex3f(end.x + Y.x, end.y + Y.y, 0.f);
        glTexCoord2f(-0.f, -1.f);
        glVertex3f(end.x - Y.x, end.y - Y.y, 0.f);

        glTexCoord2f(-1.f, +1.f);
        glVertex3f(end.x - X.x + Y.x, end.y - X.y + Y.y, 0.f);
        glTexCoord2f(-1.f, -1.f);
        glVertex3f(end.x - X.x - Y.x, end.y - X.y - Y.y, 0.f);
        glEnd();
    }
}

void FontView::onResolutionChanged(ivec2 const& res) {
    m_texture->setSize(res);
    m_font->getTextureAtlas().setResolution(res);
}

void FontView::onAtlasUpdate() {
    m_requires_update = true;
}

} // namespace bembel::tools