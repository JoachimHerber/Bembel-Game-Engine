#include "font-view.hpp"

#include <iostream>

FontView::FontView(bembel::AssetManager* asset_mgr)
: asset_mgr(asset_mgr)
, resolution(1024, 1024) {
    this->initFBO();

    this->shader.request(asset_mgr, "gen_dist_field.glsl");

    //_resolutionChangedSignal.AddSlot(this, &FontRenderer::OnResolutionChanged);
}

FontView::~FontView() {
    if(this->font) {
        auto& atlas = this->font->getTextureAtlas();
        atlas.getUpdateSignal().removeSlot(this, &FontView::onAtlasUpdate);
    }
}

void FontView::setFont(FontFamily* font) {
    if(this->font) {
        auto& atlas = this->font->getTextureAtlas();
        atlas.getUpdateSignal().removeSlot(this, &FontView::onAtlasUpdate);
    }
    this->font = font;

    if(this->font) {
        auto& atlas = this->font->getTextureAtlas();
        atlas.getUpdateSignal().addSlot(this, &FontView::onAtlasUpdate);
        atlas.setResolution(this->resolution.x, this->resolution.y);
    }
}

void FontView::update() {
    if(!this->requires_update) return;

    glUseProgram(0);

    float oldViewport[4];
    this->fbo->beginRenderToTexture();

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glDisable(GL_SCISSOR_TEST);
    glViewport(0, 0, this->resolution.x, this->resolution.y);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.f, 1.f, 0.f, 1.f, -1.f, 1.f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glScalef(1.f / this->resolution.x, 1.f / this->resolution.y, 1.f);

    this->renderToTexture();

    glEnable(GL_SCISSOR_TEST);
    this->fbo->endRenderToTexture();
    glDisable(GL_SCISSOR_TEST);

    this->requires_update = false;
}

void FontView::renderToTexture() {
    if(this->font == nullptr) return;

    glDisable(GL_BLEND);
    glLineWidth(1);
    glColor4f(0, 1, 0, 1);
    glBegin(GL_LINES);

    for(int i = 0; i < this->resolution.y; ++i) {
        float y = (i + 0.5f);

        IntersectionList intersections;
        this->getIntersections(y, this->font->getTextureAtlas().getRoot(), intersections);

        std::sort(
            intersections.begin(),
            intersections.end(),
            [](const Intersection& first, const Intersection& second) {
                return first.pos < second.pos;
            });

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

    this->shader->use();
    this->drawGlypeOutline(this->font->getTextureAtlas().getRoot());
    glUseProgram(0);

    glBlendEquation(GL_FUNC_ADD);
}

void FontView::draw(const glm::ivec2& viewport_position, const glm::uvec2& viewport_size) {
    if(!this->font) return;

    this->update();

    this->fbo->blitToBackBuffer(
        {0, 0}, this->resolution, viewport_position, viewport_position + glm::ivec2(viewport_size));

    glViewport(viewport_position.x, viewport_position.y, viewport_size.x, viewport_size.y);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.f, 1.f, 0.f, 1.f, -1.f, 1.f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glScalef(1.f / this->resolution.x, 1.f / this->resolution.y, 1.f);

    glUseProgram(0);
    glLineWidth(1);
    glBegin(GL_LINES);
    for(const auto& glyph : this->font->getGlyphs()) {
        const auto& min = glyph.getTexCoordMin();
        const auto& max = glyph.getTexCoordMax();

        glColor3f(0, 0, 1);
        glVertex3f(min.x, min.y, 0.f);
        glVertex3f(min.x, max.y, 0.f);
        glVertex3f(max.x, min.y, 0.f);
        glVertex3f(max.x, max.y, 0.f);

        glVertex3f(min.x, min.y, 0.f);
        glVertex3f(max.x, min.y, 0.f);
        glVertex3f(min.x, max.y, 0.f);
        glVertex3f(max.x, max.y, 0.f);

        glm::vec2 origin = -glyph.getExtendsMin();
        origin /= glyph.getSize();
        origin = glm::clamp(origin, glm::vec2(0), glm::vec2(1));
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
    this->texture = std::make_unique<bembel::Texture>(GL_TEXTURE_2D);
    this->texture->init(this->resolution);

    this->fbo = std::make_unique<bembel::kernel::FrameBufferObject>();
    this->fbo->setColorAttechment(0, this->texture.get());
    this->fbo->init();
}

void FontView::getIntersections(
    double y, const TextureAtlasNode* node, IntersectionList& intersections) {
    if(!node) return; // node dos not exist
    if(node->getPos().y > y || node->getPos().y + node->getSize().y < y)
        return; // the y-position is outside of the node area

    this->getIntersections(y, node->getFirstChildNode(), intersections);
    this->getIntersections(y, node->getSecondChildNode(), intersections);

    auto glyph = node->getGlyph();
    if(!glyph) return;

    y -= node->getPos().y;
    y /= node->getSize().y;
    y *= glyph->getSize().y;
    y += glyph->getExtendsMin().y;

    for(auto& line : glyph->getOutline()) {
        if(line.end.y < y && line.start.y < y) continue;
        if(line.end.y > y && line.start.y > y) continue;

        glm::vec2 dir = line.end - line.start;

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

void FontView::drawGlypeOutline(const TextureAtlasNode* node) {
    if(!node) return;

    this->drawGlypeOutline(node->getFirstChildNode());
    this->drawGlypeOutline(node->getSecondChildNode());

    auto glyph = node->getGlyph();
    if(!glyph) return;

    for(auto& it : glyph->getOutline()) {
        glm::dvec2 start = it.start;
        glm::dvec2 end   = it.end;

        start -= glm::dvec2(glyph->getExtendsMin());
        end -= glm::dvec2(glyph->getExtendsMin());
        start /= glm::dvec2(glyph->getSize());
        end /= glm::dvec2(glyph->getSize());
        start *= glm::dvec2(glyph->getTexCoordMax() - glyph->getTexCoordMin());
        end *= glm::dvec2(glyph->getTexCoordMax() - glyph->getTexCoordMin());
        start += glm::dvec2(glyph->getTexCoordMin());
        end += glm::dvec2(glyph->getTexCoordMin());

        glm::dvec2 X = this->line_width * glm::normalize(start - end);
        glm::dvec2 Y = glm::vec2(X.y, -X.x);

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

void FontView::onResolutionChanged(const glm::ivec2& res) {
    this->texture->setSize(res);
    this->font->getTextureAtlas().setResolution(res.x, res.y);
}

void FontView::onAtlasUpdate() {
    this->requires_update = true;
}
