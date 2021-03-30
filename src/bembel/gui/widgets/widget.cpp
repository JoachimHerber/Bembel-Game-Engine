#include "./widget.hpp"

#include "../graphical-user-interface.hpp"

namespace bembel::gui {

Widget::Widget(GraphicalUserInterface* gui, Widget* parent)
: gui{gui}
, parent{parent} {};
Widget::~Widget() {
    if(!this->name.empty()) { this->gui->named_widgets.erase(this->name); }
};

glm::ivec2 Widget::getMinSize() const {
    return glm::ivec2(0, 0);
}

glm::ivec2 Widget::getMaxSize() const {
    return glm::ivec2(std::numeric_limits<int>::max(), std::numeric_limits<int>::max());
}

WidgetView* Widget::getView() {
    return this->view.get();
}

bool Widget::setName(const std::string& name) {
    auto& named_widgets = this->gui->named_widgets;
    if(named_widgets.find(name) != named_widgets.end()) { return false; }

    named_widgets.emplace(name, this);
    return true;
}

GraphicalUserInterface* Widget::getGUI() const {
    return this->gui;
}

Style* Widget::getStyle() const {
    return this->gui->getRenderer().getStyle();
}

const Widget* Widget::getParentWidget() const {
    return this->parent;
}

const std::vector<Widget*>& Widget::getChildWidgets() const {
    return this->child_widgets;
}

const std::vector<InteractionHandle*>& Widget::getInteractionHandles() const {
    return this->interaction_handles;
}

} // namespace bembel::gui
