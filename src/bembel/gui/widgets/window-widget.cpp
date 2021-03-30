#include "window-widget.hpp"

#include <bembel/base/utils/utils.hpp>
#include <bembel/kernel/assets/asset-manager.hpp>
#include <bembel/kernel/display/cursor-icon.hpp>

#include "../graphical-user-interface.hpp"
#include "./button-widget.hpp"
#include "./group-widget.hpp"
#include "./widget-type-utils.hpp"

namespace bembel::gui {

const std::string WindowWidget::WIDGET_TYPE_NAME = "Window";

namespace {
    BEMBEL_STATIC_INITIALIZATION {
        WidgetTypeUtils::registerWidgetType<WindowWidget>(WindowWidget::WIDGET_TYPE_NAME);
    }
} // namespace

WindowWidget::WindowWidget(GraphicalUserInterface* gui, Widget* parent)
: Widget{gui, parent}
, top_handle{this}
, resize_handles{{{this}, {this}, {this}, {this}, {this}}} {
    this->interaction_handles.push_back(&this->top_handle);
    this->interaction_handles.push_back(&(this->resize_handles[0]));
    this->interaction_handles.push_back(&(this->resize_handles[1]));
    this->interaction_handles.push_back(&(this->resize_handles[2]));
    this->interaction_handles.push_back(&(this->resize_handles[3]));
    this->interaction_handles.push_back(&(this->resize_handles[4]));

    auto&      asset_mgr        = this->gui->getAssetManager();
    const auto hand_cursor      = asset_mgr.getAssetHandle<kernel::CursorIcon>("Hand");
    const auto h_resize_cursor  = asset_mgr.getAssetHandle<kernel::CursorIcon>("HResize");
    const auto v_resize_cursor  = asset_mgr.getAssetHandle<kernel::CursorIcon>("VResize");
    const auto crosshair_cursor = asset_mgr.getAssetHandle<kernel::CursorIcon>("Crosshair");

    this->top_handle.setCursor(hand_cursor);
    this->resize_handles[0].setCursor(h_resize_cursor);
    this->resize_handles[1].setCursor(crosshair_cursor);
    this->resize_handles[2].setCursor(v_resize_cursor);
    this->resize_handles[3].setCursor(crosshair_cursor);
    this->resize_handles[4].setCursor(h_resize_cursor);

    this->title_bar = std::make_unique<GroupWidget>(gui, this);
    this->child_widgets.push_back(this->title_bar.get());

    this->window_area = std::make_unique<GroupWidget>(gui, this);
    this->child_widgets.push_back(this->window_area.get());

    this->size.change_signal.addSlot(this, &WindowWidget::onSizeChanged);

    this->top_handle.dragging_signal.addSlot(this, &WindowWidget::moveWidget);
    this->resize_handles[0].dragging_signal.addSlot(this, &WindowWidget::onLeftResizeHandleMoved);
    this->resize_handles[1].dragging_signal.addSlot(
        this, &WindowWidget::onBottomLeftResizeHandleMoved);
    this->resize_handles[2].dragging_signal.addSlot(this, &WindowWidget::onBottomResizeHandleMoved);
    this->resize_handles[3].dragging_signal.addSlot(
        this, &WindowWidget::onBottomRightResizeHandleMoved);
    this->resize_handles[4].dragging_signal.addSlot(this, &WindowWidget::onRightResizeHandleMoved);

    this->updateLayout();
}

WindowWidget::~WindowWidget() {
}

bool WindowWidget::init() {
    this->view = std::make_unique<SimpleWindowWidgetView>(this);

    return true;
}

bool WindowWidget::init(const base::xml::Element* properties) {
    if(!this->title_bar->init(properties->FirstChildElement("TileBar"))) return false;
    if(!this->window_area->init(properties->FirstChildElement("WindowArea"))) return false;

    glm::vec2 window_size;
    if(base::xml::getAttribute(properties, "size", window_size)) size.set(window_size);

    this->view = std::make_unique<SimpleWindowWidgetView>(this);

    return true;
}

const std::string& WindowWidget::getWidgetTypeName() const {
    return WIDGET_TYPE_NAME;
}

GroupWidget* WindowWidget::getTitleBar() {
    return this->title_bar.get();
}

GroupWidget* WindowWidget::getWindowArea() {
    return this->window_area.get();
}

void WindowWidget::onSizeChanged(const glm::ivec2&, const glm::ivec2&) {
    updateLayout();
}

void WindowWidget::moveWidget(glm::ivec2& cursor_offset) {
    glm::ivec2 position = this->position.get() + cursor_offset;

    glm::ivec2 clamped_position = position;
    if(clamped_position.x < 0) clamped_position.x = 0;
    if(clamped_position.y < 0) clamped_position.y = 0;

    glm::ivec2 max_pos = this->getParentWidget()->size.get() - this->size.get();

    if(clamped_position.x > max_pos.x) clamped_position.x = max_pos.x;
    if(clamped_position.y > max_pos.y) clamped_position.y = max_pos.y;

    cursor_offset = position - clamped_position;
    this->position.set(clamped_position);
}

void WindowWidget::onLeftResizeHandleMoved(glm::ivec2& cursor_offset) {
    auto style = this->getStyle();
    assert(style && "GUI::Style is undefined");

    glm::ivec2 position = this->position.get();
    glm::ivec2 size     = this->size.get();

    int max_x = position.x + size.x;
    max_x -= this->window_area->getMinSize().x;
    max_x -= 2 * style->getValue(Style::Values::WINDOW_BORDER_WIDTH);

    int start_x   = position.x + cursor_offset.x;
    int clamped_x = std::clamp(start_x, 0, max_x);

    cursor_offset.x = start_x - clamped_x;
    this->size.set({position.x + size.x - clamped_x, size.y});
    this->position.set({clamped_x, position.y});
}

void WindowWidget::onBottomLeftResizeHandleMoved(glm::ivec2& cursor_offset) {
    auto style = this->getStyle();
    assert(style && "GUI::Style is undefined");

    float border_width     = style->getValue(Style::Values::WINDOW_BORDER_WIDTH);
    float title_bar_height = style->getValue(Style::Values::WINDOW_TITLE_BAR_HEIGHT);

    glm::ivec2 position = this->position.get();
    glm::ivec2 size     = this->size.get();

    glm::ivec2 max = position + size;
    max.x -= this->window_area->getMinSize().x;
    max.x -= 2 * border_width;
    max.y -= title_bar_height;
    max.y -= this->window_area->getMinSize().y;
    max.y -= border_width;

    glm::ivec2 start   = position + cursor_offset;
    glm::ivec2 clamped = glm::clamp(start, glm::ivec2(0), max);

    cursor_offset = start - clamped;
    this->size.set(position + size - clamped);
    this->position.set(clamped);
}

void WindowWidget::onBottomResizeHandleMoved(glm::ivec2& cursor_offset) {
    auto style = this->getStyle();
    assert(style && "GUI::Style is undefined");

    float border_width     = style->getValue(Style::Values::WINDOW_BORDER_WIDTH);
    float title_bar_height = style->getValue(Style::Values::WINDOW_TITLE_BAR_HEIGHT);

    glm::ivec2 position = this->position.get();
    glm::ivec2 size     = this->size.get();

    int max_y = position.y + size.y;
    max_y -= title_bar_height;
    max_y -= this->window_area->getMinSize().y;
    max_y -= border_width;

    int start_y   = position.y + cursor_offset.y;
    int clamped_y = std::clamp(start_y, 0, max_y);

    cursor_offset.y = start_y - clamped_y;
    this->size.set({size.x, position.y + size.y - clamped_y});
    this->position.set({position.x, clamped_y});
}

void WindowWidget::onBottomRightResizeHandleMoved(glm::ivec2& cursor_offset) {
    auto style = this->getStyle();
    assert(style && "GUI::Style is undefined");

    float border_width     = style->getValue(Style::Values::WINDOW_BORDER_WIDTH);
    float title_bar_height = style->getValue(Style::Values::WINDOW_TITLE_BAR_HEIGHT);

    glm::ivec2 position = this->position.get();
    glm::ivec2 size     = this->size.get();

    const int min_width = this->window_area->getMinSize().x + 2 * border_width;

    glm::ivec2 min = {position.x + min_width, 0};
    glm::ivec2 max;

    max.x = this->getParentWidget()->size.get().x;
    max.y = position.y + size.y;
    max.y -= title_bar_height;
    max.y -= this->window_area->getMinSize().y;
    max.y -= border_width;

    glm::ivec2 start = {position.x + size.x + cursor_offset.x, position.y + cursor_offset.y};

    glm::ivec2 clamped = glm::clamp(start, min, max);

    cursor_offset = start - clamped;

    this->size.set({clamped.x - position.x, position.y + size.y - clamped.y});
    this->position.set({position.x, clamped.y});
}

void WindowWidget::onRightResizeHandleMoved(glm::ivec2& cursor_offset) {
    auto style = this->getStyle();
    assert(style && "GUI::Style is undefined");

    float      border_width = style->getValue(Style::Values::WINDOW_BORDER_WIDTH);
    glm::ivec2 position     = this->position.get();
    glm::ivec2 size         = this->size.get();

    const int min_width = this->window_area->getMinSize().x + 2 * border_width;
    const int min_x     = position.x + min_width;
    const int max_x     = this->getParentWidget()->size.get().x;

    int start_x   = position.x + size.x + cursor_offset.x;
    int clamped_x = std::clamp(start_x, min_x, max_x);

    cursor_offset.x = start_x - clamped_x;
    this->size.set({clamped_x - position.x, size.y});
}

void WindowWidget::updateLayout() {
    auto style = this->getStyle();
    assert(style && "GUI::Style is undefined");

    float border_width     = style->getValue(Style::Values::WINDOW_BORDER_WIDTH);
    float title_bar_height = style->getValue(Style::Values::WINDOW_TITLE_BAR_HEIGHT);

    int width  = size.get().x;
    int height = size.get().y;

    int window_area_height = height - border_width - title_bar_height;
    int window_area_width  = width - 2 * border_width;

    this->title_bar->position.set({border_width, height - title_bar_height});
    this->title_bar->size.set({width - 2 * border_width, title_bar_height});

    // title bar handle
    this->top_handle.setPosition({0, height - title_bar_height});
    this->top_handle.setSize({width, title_bar_height});

    this->window_area->position.set({border_width, border_width});
    this->window_area->size.set({window_area_width, window_area_height});

    // left resize handle
    this->resize_handles[0].setPosition({0, border_width});
    this->resize_handles[0].setSize({border_width, window_area_height});

    // bottom left resize handle
    this->resize_handles[1].setPosition({0, 0});
    this->resize_handles[1].setSize({border_width, border_width});

    // bottom center resize handle
    this->resize_handles[2].setPosition({border_width, 0});
    this->resize_handles[2].setSize({window_area_width, border_width});

    // bottom right resize handle
    this->resize_handles[3].setPosition({width - border_width, 0});
    this->resize_handles[3].setSize({border_width, border_width});

    // right resize handle
    this->resize_handles[4].setPosition({width - border_width, border_width});
    this->resize_handles[4].setSize({border_width, height - title_bar_height});
}

bembel::gui::SimpleWindowWidgetView::SimpleWindowWidgetView(WindowWidget* window)
: window{window} {
}

void SimpleWindowWidgetView::draw(RenderBatchInterface& batch) {
    auto style = this->window->getStyle();
    assert(style && "GUI::Style is undefined");

    float border_width     = style->getValue(Style::Values::WINDOW_BORDER_WIDTH);
    float title_bar_height = style->getValue(Style::Values::WINDOW_TITLE_BAR_HEIGHT);

    using Colors = Style::Colors;

    float x0 = this->window->position.get().x;
    float x1 = x0 + border_width;
    float x3 = x0 + this->window->size.get().x;
    float x2 = x3 - border_width;
    float y0 = this->window->position.get().y;
    float y1 = y0 + border_width;
    float y3 = y0 + this->window->size.get().y;
    float y2 = y3 - title_bar_height;

    batch.setColor(style->getColor(Colors::WINDOW_BACKGROUND));
    batch.drawRectangle({x1, y1}, {x2, y2});

    batch.setColor(style->getColor(Colors::WINDOW_BORDER));
    batch.drawRectangle({x1, y0}, {x2, y1});
    batch.drawRectangle({x0, y0}, {x1, y3});
    batch.drawRectangle({x2, y0}, {x3, y3});

    batch.setColor(style->getColor(Colors::MENU_BAR_BACKGROUND));
    batch.drawRectangle({x1, y2}, {x2, y3});
}

} // namespace bembel::gui
