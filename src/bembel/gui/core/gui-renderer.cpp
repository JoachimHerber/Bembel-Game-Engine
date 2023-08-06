module;
#include <glbinding/gl/gl.h>
module bembel.gui.core;

import bembel.base;
import bembel.kernel;

namespace bembel::gui {
using namespace bembel::base;
using namespace bembel::kernel;
using namespace ::gl;

Renderer::Renderer(Widget& root_widget) : m_root_widget{root_widget} {}

Renderer::~Renderer() {}

bool Renderer::init(In<xml::Element const*> properties) {
    if(!properties) return false;

    m_shader.request(properties->FirstChildElement("ShaderProgram"));
    m_style.request(properties->FirstChildElement("GuiStyle"));

    if(!m_shader || !m_style) return false;

    m_batch.init();
    return true;
}

bool Renderer::init(In<Asset<ShaderProgram>> shader, In<Asset<Style>> style) {
    m_shader = shader;
    m_style  = style;

    if(!m_shader || !m_style) return false;

    m_batch.init();
    return true;
}

void Renderer::drawGui(ivec2 viewport_position, uvec2 viewport_size) {
    auto style  = m_style.get();
    auto shader = m_shader.get();

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

    m_batch.setFont(style->getFont());
    drawWidget(&m_root_widget, vec2(0, 0), vec2(0, 0), viewport_size, 0);

    shader->use();
    glUniform1i(shader->getUniformLocation("uFontTexture"), 0);
    glUniform1i(shader->getUniformLocation("uAtlasTexture"), 1);

    fontTexture->bind();
    glActiveTexture(GL_TEXTURE1);
    atlasTexture->bind();

    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glUniform2f(shader->getUniformLocation("uViewPortSize"), viewport_size.x, viewport_size.y);

    m_batch.draw();

    atlasTexture->release();
    glActiveTexture(GL_TEXTURE0);
    fontTexture->release();
}

Style* Renderer::getStyle() {
    return m_style.get();
}

void Renderer::drawWidget(
    Widget* widget, ivec2 parent_pos, ivec2 area_min, ivec2 area_max, int layer
) {
    if(widget->isHidden()) return;

    m_batch.setPositionOffset(parent_pos);

    const ivec2 pos        = parent_pos + widget->position.get();
    const ivec2 widget_min = max(area_min, pos);
    const ivec2 widget_max = min(area_max, pos + widget->size.get());
    m_batch.setDrawArea(widget_min, widget_max);

    auto view = widget->getView();
    if(view) view->draw(m_batch);

    ++layer;
    for(auto& child_widget : widget->getChildWidgets())
        drawWidget(child_widget, pos, widget_min, widget_max, layer);
}

} // namespace bembel::gui
