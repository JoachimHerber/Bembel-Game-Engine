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

FontTextureGenerator::FontTextureGenerator() {
    m_shader.request("gen_dist_field.glsl");

    m_texture = std::make_unique<Texture>(Texture::Target::TEXTURE_2D);
    m_texture->init(uvec2{m_resolution});

    m_fbo = std::make_unique<FrameBufferObject>();
    m_fbo->setColorAttechment(0, m_texture.get());
    m_fbo->init();
}

void FontTextureGenerator::setResolution(uint resolution) {
    m_resolution = resolution;
    m_texture->setSize(uvec2{resolution});
}

void FontTextureGenerator::generateTexture(FontFamily& font, GlyphTextureAtlas& texture_atlas, uint max_dist) {
    glUseProgram(0);

    float oldViewport[4];
    m_fbo->beginRenderToTexture();

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glDisable(GL_SCISSOR_TEST);
    glViewport(0, 0, m_resolution, m_resolution);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.f, 1.f, 0.f, 1.f, -1.f, 1.f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glScalef(1.f / m_resolution, 1.f / m_resolution, 1.f);

    renderToTexture(font, texture_atlas, max_dist);

    glEnable(GL_SCISSOR_TEST);
    m_fbo->endRenderToTexture();
    glDisable(GL_SCISSOR_TEST);
}

bool FontTextureGenerator::saveTexture(std::filesystem::path path) {
    std::vector<ColorRGB> buffer;
    buffer.resize(m_resolution * m_resolution);

    m_texture->bind();
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, buffer.data());
    m_texture->release();

    Image output{m_resolution, m_resolution, 1};

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

void FontTextureGenerator::renderToTexture(FontFamily& font, GlyphTextureAtlas& texture_atlas, uint max_dist) {
    glDisable(GL_BLEND);
    glLineWidth(1);
    glColor4f(0, 1, 0, 1);
    glBegin(GL_LINES);

    for(int i = 0; i < m_resolution; ++i) {
        Intersections intersections;
        getIntersections(i, texture_atlas.getRoot(), intersections);

        std::sort(
            intersections.begin(),
            intersections.end(),
            [](Intersection const& first, Intersection const& second) { return first.pos < second.pos; }
        );

        float        start = 0.0f;
        int          c     = 0;
        Intersection last  = {-1, false};
        for(auto& it : intersections) {
            if(abs(last.pos - it.pos) < 0.01 && last.direction == it.direction) continue; // ignore duplicates.

            if(c == 0) start = it.pos;

            c += it.direction ? 1 : -1;

            if(c == 0) {
                glVertex3f(start, i + 0.5, 0);
                glVertex3f(it.pos, i + 0.5, 0);
            }
            last = it;
        }
    }
    glEnd();

    glEnable(GL_BLEND);
    glBlendEquation(GL_MAX);

    m_shader->use();
    drawGlypeOutline(texture_atlas.getRoot(), max_dist * double(texture_atlas.getScaleFactor()));
    glUseProgram(0);

    glBlendEquation(GL_FUNC_ADD);
}

void FontTextureGenerator::drawGlypeOutline(TextureAtlasNode const* node, double line_width) {
    if(!node) return;

    drawGlypeOutline(node->getFirstChildNode(), line_width);
    drawGlypeOutline(node->getSecondChildNode(), line_width);

    auto glyph = node->getGlyph();
    if(!glyph) return;

    for(auto& contour : glyph->getOutline()) {
        dvec2 start = contour.back();

        start -= dvec2(glyph->getExtendsMin());
        start /= dvec2(glyph->getSize());
        start *= dvec2(glyph->getTexCoordMax() - glyph->getTexCoordMin());
        start += dvec2(glyph->getTexCoordMin());

        for(dvec2 end : contour) {
            end -= dvec2(glyph->getExtendsMin());
            end /= dvec2(glyph->getSize());
            end *= dvec2(glyph->getTexCoordMax() - glyph->getTexCoordMin());
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

            start = end;
        }
    }
}

void FontTextureGenerator::getIntersections(int row, TextureAtlasNode const* node, Intersections& intersections) {
    if(!node) return; // node doesn't exist
    if(row < node->getPos().y || node->getPos().y + node->getSize().y < row)
        return; // the y-position is outside of the node area

    getIntersections(row, node->getFirstChildNode(), intersections);
    getIntersections(row, node->getSecondChildNode(), intersections);

    auto glyph = node->getGlyph();
    if(!glyph) return;

    double y = row + 0.5 - node->getPos().y;
    y        = y * glyph->getSize().y / node->getSize().y;
    y += glyph->getExtendsMin().y;

    auto mapX = [&](double x) {
        x -= glyph->getExtendsMin().x;
        x /= glyph->getSize().x;
        x *= node->getSize().x;
        x += node->getPos().x;
        return x;
    };

    for(auto& contour : glyph->getOutline()) {
        uint n = contour.size();
        for(uint i = 0; i < n; ++i) {
            ivec2 start = contour[i];
            ivec2 end   = contour[(i + 1) % n];
            if(start.y == y) {
                ivec2 prev = contour[(i + n - 1) % n];
                if(prev.y == y) continue;
                for(int j = 2; end.y == y && i < n; ++i) end = contour[(i + j) % n];
                if(prev.y < y && y < end.y) intersections.emplace_back(mapX(start.x), false);
                if(end.y < y && y < prev.y) intersections.emplace_back(mapX(start.x), true);
            }
            else if((start.y < y && y < end.y) || (end.y < y && y < start.y)) {
                vec2 dir = end - start;

                double dist = glm::length(dir);
                dir /= dist;

                double f = (y - start.y) / dir.y;
                double x = f * dir.x + start.x;

                intersections.emplace_back(mapX(x), dir.y < 0 ? true : false);
            }
        }
        if(intersections.size() % 2 != 0) {
            log().error(""); //
        }
    }
}

} // namespace bembel::tools
