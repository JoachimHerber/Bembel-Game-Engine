#include "./renderer.hpp"

#include <bembel/kernel/assets/asset-manager.hpp>
#include <bembel/kernel/display/window.hpp>

#include "../graphical-user-interface.hpp"
#include "../rendering/widget-view.hpp"
#include "../widgets/group-widget.hpp"
#include "./style.hpp"

namespace bembel::gui {

Renderer::Renderer(GraphicalUserInterface& gui)
: gui{gui} {
}

Renderer::~Renderer() {
}

bool Renderer::init(const base::xml::Element* properties) {
    if(!properties) return false;

    auto& asset_mgr = this->gui.getAssetManager();
    this->shader.request(asset_mgr, properties->FirstChildElement("ShaderProgram"));
    this->style.request(asset_mgr, properties->FirstChildElement("GuiStyle"));

    if(!this->shader || !this->style) return false;

    this->batch.init();
    return true;
}

void Renderer::drawGui(const glm::ivec2& viewport_position, const glm::uvec2& viewport_size) {
    auto style  = this->style.get();
    auto shader = this->shader.get();

    if(style == nullptr || shader == nullptr) return;

    glViewport(viewport_position.x, viewport_position.y, viewport_size.x, viewport_size.y);

    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, viewport_size.x, 0, viewport_size.y, 1, -1);

    auto fontTexture  = style->getFontTexture();
    auto atlasTexture = style->getMainTexture();

    if(fontTexture == nullptr) return;
    if(atlasTexture == nullptr) return;

    this->drawWidget(this->gui.getRootWidget(), glm::vec2(0, 0), glm::vec2(0, 0), viewport_size, 0);

    shader->use();
    glUniform1i(shader->getUniformLocation("uFontTexture"), 0);
    // glUniform1i(shader->getUniformLocation("uTextureArray"), 1);

    fontTexture->bind();
    // atlasTexture->bind();

    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glUniform2f(shader->getUniformLocation("uViewPortSize"), viewport_size.x, viewport_size.y);

    this->batch.draw();
}

Style* Renderer::getStyle() {
    return this->style.get();
}

void Renderer::drawWidget(
    Widget*           widget,
    const glm::ivec2& parent_pos,
    const glm::ivec2& area_min,
    const glm::ivec2& area_max,
    int               layer) {
    this->batch.setPositionOffset(parent_pos);

    const glm::ivec2 pos = parent_pos + widget->position.get();
    const glm::ivec2 min = glm::max(area_min, pos);
    const glm::ivec2 max = glm::min(area_max, pos + widget->size.get());
    this->batch.setDrawArea(min, max);

    auto view = widget->getView();
    if(view) view->draw(this->batch);

    ++layer;
    for(auto& child_widget : widget->getChildWidgets())
        this->drawWidget(child_widget, pos, min, max, layer);
}

} // namespace bembel::gui
