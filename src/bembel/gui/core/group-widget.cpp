﻿module;
#include <algorithm>
#include <cctype>
#include <memory>
#include <string_view>
module bembel.gui.core;

import bembel.base;
import bembel.kernel;
import :Widget;

namespace bembel::gui {
using namespace bembel::base;
using namespace bembel::kernel;

GroupWidget::GroupWidget(In<GraphicalUserInterface*> gui) : Widget{gui} {
    size.change_signal.bind(this, &GroupWidget::onSizeChanged);
}

GroupWidget::GroupWidget(In<Widget*> parent) : Widget{parent} {
    size.change_signal.bind(this, &GroupWidget::onSizeChanged);

    m_view = std::make_unique<SimpleGroupWidgetView>(this);
}

GroupWidget::~GroupWidget() {}

bool GroupWidget::configure(xml::Element const* properties) {
    Widget::configure(properties);

    if(!properties) return true;

    std::string layout_name;
    if(xml::getAttribute(properties, "layout", layout_name)) {
        std::transform(
            layout_name.begin(),
            layout_name.end(),
            layout_name.begin(),
            [](unsigned char c) { return std::tolower(c); }
        );

        if(layout_name == "relative") {
            auto layout = this->setLayout<RelativeWidgetLayout>();
            return layout->configure(properties);
        }
        if(layout_name == "linear") {
            auto layout = this->setLayout<LinearWidgetLayout>();
            return layout->configure(properties);
        }

        // auto layout = Widget::Layout::GetLayouterFactory().createObject(layout_name);
        // if(layout->configure(properties)) setLayout(std::move(layout));
    }

    for(auto it : base::xml::IterateChildElements(properties)) {
        if(!createChildWidget(it->Value(), it)) return false;
    }
    return true;
}

Widget* GroupWidget::createChildWidget(
    std::string_view widget_type_name, xml::Element const* properties
) {
    auto widget = getFactory().createObject(widget_type_name, this);
    if(!widget) {
        logError("Unknown WidgetType {}", widget_type_name);
        return nullptr;
    }

    if(!widget->configure(properties)) return nullptr;

    m_widgets.push_back(std::move(widget));
    m_child_widgets.push_back(m_widgets.back().get());
    return m_widgets.back().get();
}

uint GroupWidget::getMinWidth(In<std::optional<uint>> height) const {
    return m_layout ? m_layout->getMinWidth(height) : 0;
}

uint GroupWidget::getMinHeight(In<std::optional<uint>> width) const {
    return m_layout ? m_layout->getMinHeight(width) : 0;
}

void GroupWidget::onSizeChanged(In<ivec2>, In<ivec2> new_size) {
    if(m_layout) m_layout->updateLayout(vec2{new_size});
}
void SimpleGroupWidgetView::draw(InOut<RenderBatchInterface> batch) {
    if(m_widget->background_color) {
        batch.setColor(*(m_widget->background_color));

        auto pos  = m_widget->position.get();
        auto size = m_widget->size.get();

        batch.drawRectangle(pos, pos + size);
    }
}

} // namespace bembel::gui
