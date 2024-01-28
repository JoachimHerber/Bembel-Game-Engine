module;
#include <algorithm>
#include <cassert>
#include <filesystem>
#include <glm/glm.hpp>
module bembel.gui.widgets;

import bembel.base;
import bembel.kernel;
import bembel.gui.core;

namespace bembel::gui {
using namespace bembel::base;
using namespace bembel::kernel;

SplitGroupWidget::SplitGroupWidget(Widget& parent, bool horizontal)
  : Widget{parent}, m_horizontal{horizontal} {
    m_child_widgets.push_back(&m_first_group);
    m_child_widgets.push_back(&m_second_group);

    m_first_group.setName("First");
    m_second_group.setName("Second");

    m_interaction_handles.push_back(&m_handle);

    m_handle.cursor = Asset<CursorIcon>(horizontal ? "HResize" : "VResize");

    this->size.change_signal.bind(this, &SplitGroupWidget::onSizeChanged);

    m_handle.dragging_signal.bind(this, &SplitGroupWidget::onMoveSeperator);

    updateLayout();

    m_view = std::make_unique<SimpleSplitGroupWidgetView>(*this);
}

SplitGroupWidget::~SplitGroupWidget() {}

bool SplitGroupWidget::configure(xml::Element const* properties) {
    auto* l = properties->FirstChildElement("Left");
    auto* r = properties->FirstChildElement("Right");
    if(l && r) {
        m_horizontal = true;
        if(!m_first_group.configure(l)) return false;
        if(!m_second_group.configure(r)) return false;
    }
    auto* b = properties->FirstChildElement("Bottom");
    auto* t = properties->FirstChildElement("Top");
    if(b && t) {
        m_horizontal = false;
        if(!m_first_group.configure(b)) return false;
        if(!m_second_group.configure(t)) return false;
    }

    xml::getAttribute(properties, "seperator_pos", m_seperator_pos);

    Widget::configure(properties);

    return true;
}

uint SplitGroupWidget::getMinWidth(In<std::optional<uint>>) const {
    auto style = getStyle();
    assert(style && "GUI::Style is undefined");

    float border_width = style->getValue(Style::Values::WINDOW_BORDER_WIDTH);

    if(m_horizontal) {
        return border_width + m_first_group.getMinWidth() + m_second_group.getMinWidth();
    } else {
        return std::max(m_first_group.getMinWidth(), m_second_group.getMinWidth());
    }
}

uint SplitGroupWidget::getMinHeight(In<std::optional<uint>>) const {
    auto style = getStyle();
    assert(style && "GUI::Style is undefined");

    float border_width = style->getValue(Style::Values::WINDOW_BORDER_WIDTH);

    if(m_horizontal) {
        return std::max(m_first_group.getMinHeight(), m_second_group.getMinHeight());
    } else {
        return border_width + m_first_group.getMinHeight() + m_second_group.getMinHeight();
    }
}

void SplitGroupWidget::onSizeChanged(In<ivec2>, In<ivec2>) {
    setSeperatorPos(m_seperator_pos);
}

void SplitGroupWidget::onMoveSeperator(In<ivec2> cursor, InOut<ivec2>) {
    auto style = getStyle();
    assert(style && "GUI::Style is undefined");

    float const border_width = style->getValue(Style::Values::WINDOW_BORDER_WIDTH);

    vec2 const pos = m_handle.position + cursor;

    m_seperator_pos = (m_horizontal ? pos.x : pos.y) - border_width/2;

    updateLayout();
}
void SplitGroupWidget::setSeperatorPos(int i) {
    auto style = getStyle();
    assert(style && "GUI::Style is undefined");

    float const border_width = style->getValue(Style::Values::WINDOW_BORDER_WIDTH);

    auto size = this->size.get();

    if(m_horizontal) {
        m_seperator_pos = std::max<int>(
            std::min<int>(i, size.x - border_width - m_second_group.getMinWidth(size.y)),
            m_first_group.getMinWidth(size.y)
        );
    } else {
        m_seperator_pos = std::max<int>(
            std::min<int>(i, size.y - border_width - m_second_group.getMinHeight(size.x)),
            m_first_group.getMinHeight(size.x)
        );
    }

    updateLayout();
}

void SplitGroupWidget::updateLayout() {
    auto style = getStyle();
    assert(style && "GUI::Style is undefined");

    float border_width = style->getValue(Style::Values::WINDOW_BORDER_WIDTH);

    int width  = this->size.get().x;
    int height = this->size.get().y;

    int min_seperator_pos =
        m_horizontal ? m_first_group.getMinWidth() : m_first_group.getMinHeight();
    int max_seperator_pos =
        (m_horizontal ? width : height) - border_width
        - (m_horizontal ? m_second_group.getMinWidth() : m_second_group.getMinHeight());

    m_seperator_pos =
        std::max<int>(std::min<int>(m_seperator_pos, max_seperator_pos), min_seperator_pos);

    if(m_horizontal) {
        m_first_group.position  = ivec2{0 /**************************/, 0};
        m_handle.position       = ivec2{m_seperator_pos /************/, 0};
        m_second_group.position = ivec2{m_seperator_pos + border_width, 0};

        m_first_group.size  = ivec2{/*******************/ m_seperator_pos, height};
        m_handle.size       = ivec2{/*****/ border_width /***************/, height};
        m_second_group.size = ivec2{width - border_width - m_seperator_pos, height};
    } else {
        m_first_group.position  = ivec2{0, 0};
        m_handle.position       = ivec2{0, m_seperator_pos};
        m_second_group.position = ivec2{0, m_seperator_pos + border_width};

        m_first_group.size  = ivec2{width, /*********************/ m_seperator_pos};
        m_handle.size       = ivec2{width, /******/ border_width /***************/};
        m_second_group.size = ivec2{width, height - border_width - m_seperator_pos};
    }
}

void SimpleSplitGroupWidgetView::draw(RenderBatchInterface& batch) {
    auto style = m_widget.getStyle();
    assert(style && "GUI::Style is undefined");

    float border_width = style->getValue(Style::Values::WINDOW_BORDER_WIDTH);

    if(m_widget.isSelected()) {
        batch.setPrimaryColor(style->getColor(Style::Colors::BORDER_ACTIVE));
    } else if(m_widget.isHovered()) {
        batch.setPrimaryColor(style->getColor(Style::Colors::BORDER_HOVERED));
    } else {
        batch.setPrimaryColor(style->getColor(Style::Colors::BORDER));
    }

    vec2 min = m_widget.position.get();
    vec2 max = min + vec2(m_widget.size.get());

    if(m_widget.isHorizontal()) {
        min.x = m_widget.getSeperatorPos();
        max.x = m_widget.getSeperatorPos() + border_width;
    } else {
        min.y = m_widget.getSeperatorPos();
        max.y = m_widget.getSeperatorPos() + border_width;
    }
    batch.drawRectangle(min, max);
}

} // namespace bembel::gui
