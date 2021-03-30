#include "./labeled-button-widget.hpp"

#include <bembel/base/utils/utils.hpp>
#include <bembel/kernel/assets/asset-manager.hpp>
#include <bembel/kernel/display/cursor-icon.hpp>

#include "../graphical-user-interface.hpp"
#include "../rendering/style.hpp"
#include "./widget-type-utils.hpp"

namespace bembel::gui {

const std::string LabeledButtonWidget::WIDGET_TYPE_NAME = "LabeledButton";

namespace {
    BEMBEL_STATIC_INITIALIZATION {
        WidgetTypeUtils::registerWidgetType<LabeledButtonWidget>(
            LabeledButtonWidget::WIDGET_TYPE_NAME);
    }
} // namespace

LabeledButtonWidget::LabeledButtonWidget(GraphicalUserInterface* gui, Widget* parent)
: Widget{gui, parent}
, button{gui, this}
, label{gui, this}
, press_signal{this->button.press_signal}
, release_signal{this->button.release_signal}
, click_signal{this->button.click_signal}
, text{this->label.text} {
    this->child_widgets.push_back(&(this->button));
    this->child_widgets.push_back(&(this->label));

    this->size.change_signal.addSlot(this, &LabeledButtonWidget::onSizeChanged);
}

LabeledButtonWidget::~LabeledButtonWidget() {
}

bool LabeledButtonWidget::init() {
    return this->button.init() && this->label.init();
}

bool LabeledButtonWidget::init(const base::xml::Element* properties) {
    auto style = this->getStyle();
    assert(style && "GUI::Style is undefined");

    glm::vec2 size;
    if(base::xml::getAttribute(properties, "size", size)) this->size.set(size);

    std::string name;
    if(base::xml::getAttribute(properties, "name", name)) this->setName(name);

    std::string label;
    if(base::xml::getAttribute(properties, "label", label)) this->text = label;

    float border = style->getValue(Style::Values::BUTTON_BORDER_WIDTH);

    this->label.position = glm::ivec2(border, border);
    return this->button.init() && this->label.init();
}

InteractionHandle::State LabeledButtonWidget::getState() {
    return this->button.getState();
}

const std::string& LabeledButtonWidget::getWidgetTypeName() const {
    return WIDGET_TYPE_NAME;
}

void LabeledButtonWidget::onSizeChanged(const glm::ivec2&, const glm::ivec2& new_size) {
    auto style = this->getStyle();
    assert(style && "GUI::Style is undefined");

    float border = style->getValue(Style::Values::BUTTON_BORDER_WIDTH);

    this->button.size = new_size;
    this->label.size  = new_size - glm::ivec2(2 * border, 2 * border);
}

} // namespace bembel::gui
