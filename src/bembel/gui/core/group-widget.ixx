module;
#include "bembel/pch.h"
export module bembel.gui.core:GroupWidget;

import bembel.base;
import bembel.kernel;
import :Widget;
import :WidgetLayout;

namespace bembel::gui {
using namespace bembel::base;
using namespace bembel::kernel;

export class GroupWidget : public Widget {
  public:
    GroupWidget(GraphicalUserInterface& gui);
    GroupWidget(Widget& parent);
    ~GroupWidget();

    virtual bool configure(xml::Element const* properties);

    template <typename WidgetType>
    WidgetType* createChildWidget() {
        auto widget     = std::make_unique<WidgetType>(*(Widget*)this);
        auto widget_ptr = widget.get();
        m_widgets.push_back(std::move(widget));
        m_child_widgets.push_back(widget_ptr);
        return widget_ptr;
    }
    Widget* createChildWidget(
        std::string_view widget_type_name, xml::Element const* properties = nullptr
    );

    void setLayout(std::shared_ptr<WidgetLayout>);

    void updateLayout();

    ivec2 getMinSize() const override;

    static constexpr std::string_view WIDGET_TYPE_NAME = "Group";
    std::string_view getWidgetTypeName() const override { return WIDGET_TYPE_NAME; }

  private:
    void onSizeChanged(In<ivec2>, In<ivec2>);

  private:
    std::vector<std::unique_ptr<Widget>> m_widgets;

    std::shared_ptr<WidgetLayout> m_layout;
};

} // namespace bembel::gui
