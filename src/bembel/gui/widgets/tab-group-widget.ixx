module;
#include <memory>
#include <string_view>
export module bembel.gui.widgets:Tabs;

import bembel.base;
import bembel.kernel;
import bembel.gui.core;
import :Label;

namespace bembel::gui {
using namespace bembel::base;
using namespace bembel::kernel;

export class TabGroupWidget : public Widget {
  public:
    static constexpr std::string_view WIDGET_TYPE_NAME = "Tabs";

  public:
    TabGroupWidget(Widget& parent);
    ~TabGroupWidget();

    virtual bool configure(xml::Element const* properties) override;

    virtual uint getMinWidth() const override;
    virtual uint getMinHeight() const override;

    virtual std::string_view getWidgetTypeName() const override { return WIDGET_TYPE_NAME; }

    class Tab {
        friend class TabGroupWidget;

      public:
        Tab(TabGroupWidget* group, usize index, std::u8string_view label);
        ~Tab();

        bool isHovered() const { return m_handle.isDisabled(); }
        bool isSelected() const { return m_group->getSelectedTab() == m_index; }

        ivec2 getPosition() const { return m_handle.position; }
        ivec2 getSize() const { return m_handle.size; }

        LabelWidget& getLabel() { return m_label; }
        GroupWidget& getContentArea() { return m_content; }

      protected:
        void onAction(InteractionHandle::Action, ivec2);

      private:
        TabGroupWidget* m_group;
        usize           m_index;

        InteractionHandle m_handle;

        LabelWidget m_label;
        GroupWidget m_content;
    };

    Tab* addTab(std::u8string_view lable);

    usize getNumTabs() const { return m_tabs.size(); }
    Tab*  getTab(usize i) const { return i <= m_tabs.size() ? m_tabs[i].get() : nullptr; }

    usize getSelectedTab() const { return m_selected_tab; }
    void  setSelectedTab(usize index);

  protected:
    void onSizeChanged(In<ivec2>, In<ivec2>);

    void updateLayout();

  private:
    std::vector<std::unique_ptr<Tab>> m_tabs;
    usize                             m_selected_tab = 0;
};

export class SimpleTabGroupWidgetView : public Widget::View {
  public:
    SimpleTabGroupWidgetView(TabGroupWidget& widget) : m_widget{widget} {}
    ~SimpleTabGroupWidgetView() = default;

    void draw(RenderBatchInterface& batch) override;

  private:
    TabGroupWidget& m_widget;
};

} // namespace bembel::gui
