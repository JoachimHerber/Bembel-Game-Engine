module;
#include <utility>
#include <memory>
export module bembel.gui.core:GroupWidget;

import bembel.base;
import bembel.kernel;
import :Widget;

namespace bembel::gui {
using namespace bembel::base;
using namespace bembel::kernel;

export class GroupWidget : public Widget {
  public:
    static constexpr std::string_view WIDGET_TYPE_NAME = "Group";

  public:
    GroupWidget(GraphicalUserInterface& gui);
    GroupWidget(Widget& parent);
    ~GroupWidget();

    virtual bool configure(xml::Element const* properties);

    virtual uint getMinWidth() const override;
    virtual uint getMinHeight() const override;

    std::string_view getWidgetTypeName() const override { return WIDGET_TYPE_NAME; }

    template <typename WidgetType, typename... TArgs>
    WidgetType* createChildWidget(TArgs&&... args) {
        auto widget = std::make_unique<WidgetType>(*(Widget*)this, std::forward<TArgs>(args)...);
        auto widget_ptr = widget.get();
        m_widgets.push_back(std::move(widget));
        m_child_widgets.push_back(widget_ptr);
        return widget_ptr;
    }

    Widget* createChildWidget(
        std::string_view widget_type_name, xml::Element const* properties = nullptr
    );

    template <typename LayoutType, typename... TArgs>
    LayoutType* setLayout(TArgs&&... args)
        requires std::is_base_of_v<Widget::Layout, LayoutType>
    {
        m_layout = std::make_unique<LayoutType>(this, std::forward<TArgs>(args)...);
        return (LayoutType*)m_layout.get();
    }

    void updateLayout() {
        if(m_layout) m_layout->updateLayout();
    }

  private:
    void onSizeChanged(In<ivec2>, In<ivec2>);

  private:
    std::vector<std::unique_ptr<Widget>> m_widgets;

    std::unique_ptr<Widget::Layout> m_layout;
};

} // namespace bembel::gui
