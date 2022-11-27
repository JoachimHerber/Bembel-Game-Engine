module;
#include "bembel/pch.h"
module bembel.gui.widgets;

import bembel.base;
import bembel.kernel;
import bembel.gui.core;

namespace bembel::gui {
using namespace bembel::base;
using namespace bembel::kernel;

WindowWidget::WindowWidget(Widget& parent) : Widget{parent} {
    m_child_widgets.push_back(&m_title_bar);
    m_child_widgets.push_back(&m_window_area);

    m_title_bar.setName("Title");
    m_window_area.setName("Body");

    m_interaction_handles.push_back(&m_top_handle);
    m_interaction_handles.push_back(&m_resize_handle_left);
    m_interaction_handles.push_back(&m_resize_handle_bottom_left);
    m_interaction_handles.push_back(&m_resize_handle_bottom_center);
    m_interaction_handles.push_back(&m_resize_handle_bottom_right);
    m_interaction_handles.push_back(&m_resize_handle_right);

    auto&      asset_mgr        = m_gui.getAssetManager();
    auto const hand_cursor      = asset_mgr.getAssetHandle<CursorIcon>("Hand");
    auto const h_resize_cursor  = asset_mgr.getAssetHandle<CursorIcon>("HResize");
    auto const v_resize_cursor  = asset_mgr.getAssetHandle<CursorIcon>("VResize");
    auto const crosshair_cursor = asset_mgr.getAssetHandle<CursorIcon>("Crosshair");

    m_top_handle.cursor                  = hand_cursor;
    m_resize_handle_left.cursor          = h_resize_cursor;
    m_resize_handle_bottom_left.cursor   = crosshair_cursor;
    m_resize_handle_bottom_center.cursor = v_resize_cursor;
    m_resize_handle_bottom_right.cursor  = crosshair_cursor;
    m_resize_handle_right.cursor         = h_resize_cursor;

    this->size.change_signal.bind(this, &WindowWidget::onSizeChanged);

    m_top_handle /*******************/.dragging_signal.bind(this, &WindowWidget::moveWidget);
    m_resize_handle_left /***********/.dragging_signal.bind(
        this, &WindowWidget::onLeftResizeHandleMoved
    );
    m_resize_handle_bottom_left /****/.dragging_signal.bind(
        this, &WindowWidget::onBottomLeftResizeHandleMoved
    );
    m_resize_handle_bottom_center /**/.dragging_signal.bind(
        this, &WindowWidget::onBottomResizeHandleMoved
    );
    m_resize_handle_bottom_right /***/.dragging_signal.bind(
        this, &WindowWidget::onBottomRightResizeHandleMoved
    );
    m_resize_handle_right /**********/.dragging_signal.bind(
        this, &WindowWidget::onRightResizeHandleMoved
    );

    updateLayout();

    m_view = std::make_unique<SimpleWindowWidgetView>(*this);
}

WindowWidget::~WindowWidget() {}

bool WindowWidget::configure(xml::Element const* properties) {
    if(!m_title_bar.configure(properties->FirstChildElement("TileBar"))) return false;
    if(!m_window_area.configure(properties->FirstChildElement("WindowArea"))) return false;

    Widget::configure(properties);

    return true;
}

uint WindowWidget::getMinWidth() const {
    auto style = getStyle();
    assert(style && "GUI::Style is undefined");

    float border_width = style->getValue(Style::Values::WINDOW_BORDER_WIDTH);

    return 2 * border_width + std::max(m_window_area.getMinWidth(), m_title_bar.getMinWidth());
}

uint WindowWidget::getMinHeight() const {
    auto style = getStyle();
    assert(style && "GUI::Style is undefined");

    float border_width     = style->getValue(Style::Values::WINDOW_BORDER_WIDTH);
    float title_bar_height = style->getValue(Style::Values::WINDOW_TITLE_BAR_HEIGHT);
    return title_bar_height + border_width + m_window_area.getMinHeight();
}

void WindowWidget::onSizeChanged(In<ivec2>, In<ivec2>) {
    updateLayout();
}

void WindowWidget::moveWidget(In<ivec2>, InOut<ivec2> cursor_offset) {
    ivec2 pos = this->position.get() + cursor_offset;

    ivec2 clamped_pos = pos;
    if(clamped_pos.x < 0) clamped_pos.x = 0;
    if(clamped_pos.y < 0) clamped_pos.y = 0;

    ivec2 max_pos = m_parent->size.get() - this->size.get();

    if(clamped_pos.x > max_pos.x) clamped_pos.x = max_pos.x;
    if(clamped_pos.y > max_pos.y) clamped_pos.y = max_pos.y;

    cursor_offset  = pos - clamped_pos;
    this->position = clamped_pos;
}

void WindowWidget::onLeftResizeHandleMoved(In<ivec2>, InOut<ivec2> cursor_offset) {
    auto style = getStyle();
    assert(style && "GUI::Style is undefined");

    ivec2 position = this->position;
    ivec2 size     = this->size;

    int max_x = position.x + size.x;
    max_x -= m_window_area.getMinWidth();
    max_x -= 2 * style->getValue(Style::Values::WINDOW_BORDER_WIDTH);

    int start_x   = position.x + cursor_offset.x;
    int clamped_x = glm::clamp(start_x, 0, max_x);

    cursor_offset.x = start_x - clamped_x;
    this->size.set({position.x + size.x - clamped_x, size.y});
    this->position.set({clamped_x, position.y});
}

void WindowWidget::onBottomLeftResizeHandleMoved(In<ivec2>, InOut<ivec2> cursor_offset) {
    auto style = getStyle();
    assert(style && "GUI::Style is undefined");

    float border_width     = style->getValue(Style::Values::WINDOW_BORDER_WIDTH);
    float title_bar_height = style->getValue(Style::Values::WINDOW_TITLE_BAR_HEIGHT);

    ivec2 pos  = this->position;
    ivec2 size = this->size;

    ivec2 max = pos + size;
    max.x -= m_window_area.getMinWidth();
    max.x -= 2 * border_width;
    max.y -= title_bar_height;
    max.y -= m_window_area.getMinHeight();
    max.y -= border_width;

    ivec2 start   = pos + cursor_offset;
    ivec2 clamped = glm::clamp(start, ivec2{0}, max);

    cursor_offset = start - clamped;
    this->size.set(pos + size - clamped);
    this->position.set(clamped);
}

void WindowWidget::onBottomResizeHandleMoved(In<ivec2>, InOut<ivec2> cursor_offset) {
    auto style = this->getStyle();
    assert(style && "GUI::Style is undefined");

    float border_width     = style->getValue(Style::Values::WINDOW_BORDER_WIDTH);
    float title_bar_height = style->getValue(Style::Values::WINDOW_TITLE_BAR_HEIGHT);

    ivec2 pos  = this->position;
    ivec2 size = this->size;

    int max_y = pos.y + size.y - title_bar_height - m_window_area.getMinHeight() - border_width;

    int start_y   = pos.y + cursor_offset.y;
    int clamped_y = glm::clamp(start_y, 0, max_y);

    cursor_offset.y = start_y - clamped_y;
    this->size.set({size.x, pos.y + size.y - clamped_y});
    this->position.set({pos.x, clamped_y});
}

void WindowWidget::onBottomRightResizeHandleMoved(In<ivec2>, InOut<ivec2> cursor_offset) {
    auto style = getStyle();
    assert(style && "GUI::Style is undefined");

    float border_width     = style->getValue(Style::Values::WINDOW_BORDER_WIDTH);
    float title_bar_height = style->getValue(Style::Values::WINDOW_TITLE_BAR_HEIGHT);

    ivec2 pos  = this->position;
    ivec2 size = this->size;

    int const min_width = m_window_area.getMinWidth() + 2 * border_width;

    ivec2 min = {pos.x + min_width, 0};
    ivec2 max = {m_parent->size.get().x, pos.y + size.y};

    max.y -= title_bar_height;
    max.y -= m_window_area.getMinHeight();
    max.y -= border_width;

    glm::ivec2 start   = {pos.x + size.x + cursor_offset.x, pos.y + cursor_offset.y};
    glm::ivec2 clamped = glm::clamp(start, min, max);

    cursor_offset = start - clamped;

    this->size.set({clamped.x - pos.x, pos.y + size.y - clamped.y});
    this->position.set({pos.x, clamped.y});
}

void WindowWidget::onRightResizeHandleMoved(In<ivec2>, InOut<ivec2> cursor_offset) {
    auto style = getStyle();
    assert(style && "GUI::Style is undefined");

    float border_width = style->getValue(Style::Values::WINDOW_BORDER_WIDTH);
    ivec2 pos          = this->position;
    ivec2 size         = this->size;

    int const min_width = m_window_area.getMinWidth() + 2 * border_width;
    int const min_x     = pos.x + min_width;
    int const max_x     = m_parent->size.get().x;

    int start_x   = pos.x + size.x + cursor_offset.x;
    int clamped_x = glm::clamp(start_x, min_x, max_x);

    cursor_offset.x = start_x - clamped_x;
    this->size.set({clamped_x - pos.x, size.y});
}

void WindowWidget::updateLayout() {
    auto style = getStyle();
    assert(style && "GUI::Style is undefined");

    float border_width     = style->getValue(Style::Values::WINDOW_BORDER_WIDTH);
    float title_bar_height = style->getValue(Style::Values::WINDOW_TITLE_BAR_HEIGHT);

    int width  = this->size.get().x;
    int height = this->size.get().y;

    int window_area_height = height - border_width - title_bar_height;
    int window_area_width  = width - 2 * border_width;
    //

    m_title_bar.position                   = ivec2{/*****/ border_width, height - title_bar_height};
    m_window_area.position                 = ivec2{/*****/ border_width, /**********/ border_width};
    m_top_handle.position                  = ivec2{/****************/ 0, height - title_bar_height};
    m_resize_handle_left.position          = ivec2{/****************/ 0, /**********/ border_width};
    m_resize_handle_bottom_left.position   = ivec2{/****************/ 0, /*********************/ 0};
    m_resize_handle_bottom_center.position = ivec2{/*****/ border_width, /*********************/ 0};
    m_resize_handle_bottom_right.position  = ivec2{width - border_width, /*********************/ 0};
    m_resize_handle_right.position         = ivec2{width - border_width, /**********/ border_width};

    m_title_bar.size                   = ivec2{width - 2 * border_width, /******/ title_bar_height};
    m_window_area.size                 = ivec2{/****/ window_area_width, /****/ window_area_height};
    m_top_handle.size                  = ivec2{/****************/ width, /******/ title_bar_height};
    m_resize_handle_left.size          = ivec2{/*********/ border_width, /****/ window_area_height};
    m_resize_handle_bottom_left.size   = ivec2{/*********/ border_width, /**********/ border_width};
    m_resize_handle_bottom_center.size = ivec2{/****/ window_area_width, /**********/ border_width};
    m_resize_handle_bottom_right.size  = ivec2{/*********/ border_width, /**********/ border_width};
    m_resize_handle_right.size         = ivec2{/*********/ border_width, height - title_bar_height};
}

void SimpleWindowWidgetView::draw(RenderBatchInterface& batch) {
    auto style = m_window.getStyle();
    assert(style && "GUI::Style is undefined");

    float border_width     = style->getValue(Style::Values::WINDOW_BORDER_WIDTH);
    float title_bar_height = style->getValue(Style::Values::WINDOW_TITLE_BAR_HEIGHT);

    //  y3 ┏━┳━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┳━┓
    //     ┃ ┃                  Title Bar                  ┃ ┃
    //  y2 ┃ ┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫ ┃
    //     ┃ ┃                                             ┃ ┃
    //     ┃ ┃                                             ┃ ┃
    //     ┃ ┃                                             ┃ ┃
    //     ┃ ┃                                             ┃ ┃
    //     ┃ ┃                                             ┃ ┃
    //     ┃ ┃                 Window Area                 ┃ ┃
    //     ┃ ┃                                             ┃ ┃
    //     ┃ ┃                                             ┃ ┃
    //     ┃ ┃                                             ┃ ┃
    //     ┃ ┃                                             ┃ ┃
    //     ┃ ┃                                             ┃ ┃
    //  y1 ┃ ┡━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┩ ┃
    //  y0 ┗━┷━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┷━┛
    //    x0 x1                                           x2 x3
    float x0 = m_window.position.get().x;
    float x3 = x0 + m_window.size.get().x;
    float x2 = x3 - border_width;
    float x1 = x0 + border_width;

    float y0 = m_window.position.get().y;
    float y3 = y0 + m_window.size.get().y;
    float y2 = y3 - title_bar_height;
    float y1 = y0 + border_width;

    // Title Bar
    batch.setColor(style->getColor(Style::Colors::WINDOW_BACKGROUND));
    batch.drawRectangle({x1, y1}, {x2, y2});

    // Window Area
    batch.setColor(style->getColor(Style::Colors::MENU_BAR_BACKGROUND));
    batch.drawRectangle({x1, y2}, {x2, y3});

    // Border
    batch.setColor(style->getColor(Style::Colors::WINDOW_BORDER));
    batch.drawRectangle({x0, y0}, {x1, y3});
    batch.drawRectangle({x1, y0}, {x2, y1});
    batch.drawRectangle({x2, y0}, {x3, y3});
}

} // namespace bembel::gui
