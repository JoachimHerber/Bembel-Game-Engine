module;
#include "bembel/pch.h"
module bembel.gui.core;

import bembel.base;
import bembel.kernel;
import :Widget;

namespace bembel::gui {
using namespace bembel::base;
using namespace bembel::kernel;

GroupWidget::GroupWidget(GraphicalUserInterface& gui) : Widget{gui} {
    size.change_signal.bind(this, &GroupWidget::onSizeChanged);
}

GroupWidget::GroupWidget(Widget& parent) : Widget{parent} {
    size.change_signal.bind(this, &GroupWidget::onSizeChanged);
}

GroupWidget::~GroupWidget() {}

bool GroupWidget::configure(base::xml::Element const* properties) {
    Widget::configure(properties);

    if(!properties) return true;

    std::string layout_name;
    if(base::xml::getAttribute(properties, "layout", layout_name)) {
        std::transform(
            layout_name.begin(),
            layout_name.end(),
            layout_name.begin(),
            [](unsigned char c) { return std::tolower(c); }
        );

        auto layout = WidgetLayout::GetLayouterFactory().createObject(layout_name);
        if(layout->configure(properties)) setLayout(std::move(layout));
    }

    for(auto it : base::xml::IterateChildElements(properties)) {
        if(!createChildWidget(it->Value(), it)) return false;
    }
    return true;
}

Widget* GroupWidget::createChildWidget(
    std::string_view widget_type_name, xml::Element const* properties
) {
    auto widget = getFactory().createObject(widget_type_name, *this);
    if(!widget) {
        log().error("Unknown WidgetType {}", widget_type_name);
        return nullptr;
    }

    if(!widget->configure(properties)) return nullptr;

    m_widgets.push_back(std::move(widget));
    m_child_widgets.push_back(m_widgets.back().get());
    return m_widgets.back().get();
}

void GroupWidget::setLayout(std::shared_ptr<WidgetLayout> layout) {
    m_layout = layout;
    updateLayout();
}

void GroupWidget::updateLayout() {
    if(m_layout) m_layout->calculateLayout(this->size.get(), m_child_widgets);
}

ivec2 GroupWidget::getMinSize() const {
    if(m_layout) {
        return m_layout->calculateMinSize(m_child_widgets);
    } else {
        return {0, 0};
    }
}

void GroupWidget::onSizeChanged(In<ivec2> old_size, In<ivec2> new_size) {
    if(m_layout) m_layout->calculateLayout(new_size, m_child_widgets);
}

} // namespace bembel::gui
