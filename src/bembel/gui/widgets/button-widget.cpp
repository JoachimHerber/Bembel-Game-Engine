#include "./button-widget.hpp"

#include <bembel/base/utils/utils.hpp>
#include <bembel/kernel/assets/asset-manager.hpp>
#include <bembel/kernel/display/cursor-icon.hpp>

#include "../graphical-user-interface.hpp"
#include "../rendering/style.hpp"
#include "./widget-type-utils.hpp"

namespace bembel::gui {

const std::string ButtonWidget::WIDGET_TYPE_NAME = "Button";

namespace {
    BEMBEL_STATIC_INITIALIZATION {
        WidgetTypeUtils::registerWidgetType<ButtonWidget>(ButtonWidget::WIDGET_TYPE_NAME);
    }
} // namespace

ButtonWidget::ButtonWidget(GraphicalUserInterface* gui, Widget* parent)
: Widget{gui, parent}
, handle{this} {
    this->interaction_handles.push_back(&(this->handle));

    this->size.change_signal.addSlot(this, &ButtonWidget::onSizeChanged);

    this->handle.state_change_signal.addSlot(this, &ButtonWidget::onStateChange);
    this->handle.setCursor(this->gui->getAssetManager().getAssetHandle<kernel::CursorIcon>("Hand"));
}

ButtonWidget::~ButtonWidget() {
}

bool ButtonWidget::init() {
    this->view = std::make_unique<SimpleButtonWidgetView>(this);
    return true;
}

bool ButtonWidget::init(const base::xml::Element* properties) {
    glm::vec2 size;
    if(base::xml::getAttribute(properties, "size", size)) this->size.set(size);

    std::string name;
    if(base::xml::getAttribute(properties, "name", name)) this->setName(name);

    this->view = std::make_unique<SimpleButtonWidgetView>(this);
    return true;
}

InteractionHandle::State ButtonWidget::getState() {
    return this->handle.getState();
}

const std::string& ButtonWidget::getWidgetTypeName() const {
    return WIDGET_TYPE_NAME;
}

void ButtonWidget::onSizeChanged(const glm::ivec2&, const glm::ivec2& new_size) {
    this->handle.setSize(new_size);
}

void ButtonWidget::onStateChange(
    InteractionHandle::State old_state, InteractionHandle::State new_state) {
    if(new_state == InteractionHandle::State::ACTIVE) { this->press_signal.emit(); }
    if(old_state == InteractionHandle::State::ACTIVE) {
        if(new_state == InteractionHandle::State::SELECTED) { this->click_signal.emit(); }
        this->release_signal.emit();
    }
}

inline Style::Colors getBorderColor(InteractionHandle::State state) {
    switch(state) {
        case InteractionHandle::State::DISABLED: return Style::Colors::BUTTON_BORDER_DISABLED;
        case InteractionHandle::State::IDLE: return Style::Colors::BUTTON_BORDER;
        case InteractionHandle::State::SELECTED: return Style::Colors::BUTTON_BORDER_HOVERED;
        case InteractionHandle::State::ACTIVE: return Style::Colors::BUTTON_BORDER_ACTIVE;
    }
    return Style::Colors::BUTTON_BORDER;
}

inline Style::Colors getBackgroundColor(InteractionHandle::State state) {
    switch(state) {
        case InteractionHandle::State::DISABLED: return Style::Colors::BUTTON_BACKGROUND_DISABLED;
        case InteractionHandle::State::IDLE: return Style::Colors::BUTTON_BACKGROUND;
        case InteractionHandle::State::SELECTED: return Style::Colors::BUTTON_BACKGROUND_HOVERED;
        case InteractionHandle::State::ACTIVE: return Style::Colors::BUTTON_BACKGROUND_ACTIVE;
    }
    return Style::Colors::BUTTON_BACKGROUND;
}

SimpleButtonWidgetView::SimpleButtonWidgetView(ButtonWidget* button)
: button{button} {
}

bembel::gui::SimpleButtonWidgetView::~SimpleButtonWidgetView() {
}

void SimpleButtonWidgetView::draw(RenderBatchInterface& batch) {
    auto style = this->button->getStyle();
    assert(style && "GUI::Style is undefined");

    float border = style->getValue(Style::Values::BUTTON_BORDER_WIDTH);

    glm::vec2 outer_min = this->button->position.get();
    glm::vec2 outer_max = outer_min + glm::vec2(this->button->size.get());
    glm::vec2 inner_min = outer_min + glm::vec2(border);
    glm::vec2 inner_max = outer_max - glm::vec2(border);

    batch.setColor(style->getColor(getBorderColor(this->button->getState())));
    batch.drawRectangle({inner_min.x, outer_min.y}, {inner_max.x, inner_min.y});
    batch.drawRectangle({inner_min.x, inner_max.y}, {inner_max.x, outer_max.y});
    batch.drawRectangle({outer_min.x, outer_min.y}, {inner_min.x, outer_max.y});
    batch.drawRectangle({inner_max.x, outer_min.y}, {outer_max.x, outer_max.y});

    batch.setColor(style->getColor(getBackgroundColor(this->button->getState())));
    batch.drawRectangle(inner_min, inner_max);
}

} // namespace bembel::gui
