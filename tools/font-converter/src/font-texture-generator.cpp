module;
#include <glbinding/gl/gl.h>

#include <algorithm>
#include <glm/glm.hpp>
#include <memory>
module bembel.tools.font_converter;

import bembel;

namespace bembel::tools {
using namespace bembel::base;
using namespace bembel::kernel;
using namespace bembel::gui;
using namespace ::gl;

FontTextureGenerator::FontTextureGenerator(AssetManager& asset_mgr) : m_asset_mgr{asset_mgr} {
    m_shader.request(asset_mgr, "gen_dist_field.glsl");

    m_texture = std::make_unique<Texture>(Texture::Target::TEXTURE_2D);
    m_texture->init(m_resolution);

    m_fbo = std::make_unique<FrameBufferObject>();
    m_fbo->setColorAttechment(0, m_texture.get());
    m_fbo->init();
}

void FontTextureGenerator::setResolution(uvec2 resolution) {
    m_resolution = resolution;
    m_texture->setSize(resolution);
}

void FontTextureGenerator::generateTexture(
    FontFamily& font, GlyphTextureAtlas& texture_atlas, double line_width
) {
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

    renderToTexture(font, texture_atlas, line_width);

    glEnable(GL_SCISSOR_TEST);
    m_fbo->endRenderToTexture();
    glDisable(GL_SCISSOR_TEST);
}

bool FontTextureGenerator::saveTexture(std::filesystem::path path) {
    std::vector<ColorRGB> buffer;
    buffer.resize(m_resolution.x * m_resolution.y);

    m_texture->bind();
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, buffer.data());
    m_texture->release();

    Image output{m_resolution.x, m_resolution.y, 1};

    u8* p = output.getData();
    for(ColorRGB c : buffer) {
        float inside = /****/ (c.g / 255.0f);
        float dist   = 1.0f - (c.r / 255.0f);

        float signed_dist = std::lerp(-dist, dist, inside);

        *(p++) = 255 * (signed_dist / 2 + 0.5f);
    }

    path.replace_extension(".png");

    return output.save(path, true);
}

void FontTextureGenerator::renderToTexture(
    FontFamily& font, GlyphTextureAtlas& texture_atlas, double line_width
) {
    glDisable(GL_BLEND);
    glLineWidth(1);
    glColor4f(0, 1, 0, 1);
    glBegin(GL_LINES);

    for(int i = 0; i < m_resolution.y; ++i) {
        float y = (i + 0.5f);

        Intersections intersections;
        getIntersections(y, texture_atlas.getRoot(), intersections);

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
    drawGlypeOutline(texture_atlas.getRoot(), line_width);
    glUseProgram(0);

    glBlendEquation(GL_FUNC_ADD);
}

void FontTextureGenerator::drawGlypeOutline(TextureAtlasNode const* node, double line_width) {
    if(!node) return;

    drawGlypeOutline(node->getFirstChildNode(), line_width);
    drawGlypeOutline(node->getSecondChildNode(), line_width);

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

        dvec2 X = line_width * glm::normalize(start - end);
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

void FontTextureGenerator::getIntersections(
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

        double dist = glm::length(dir);
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

} // namespace bembel::tools
