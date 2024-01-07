module;
#include <string>
export module bembel.gui.widgets:SplitGroup;

import bembel.base;
import bembel.kernel;
import bembel.gui.core;

namespace bembel::gui {
using namespace bembel::base;
using namespace bembel::kernel;

export class SplitGroupWidget : public Widget {
  public:
    static constexpr std::string_view WIDGET_TYPE_NAME = "ResizableTable";

  public:
    SplitGroupWidget(Widget& parent, bool horizontal = true);
    ~SplitGroupWidget();

    virtual bool configure(xml::Element const* properties) override;

    virtual uint getMinWidth(In<std::optional<uint>>) const override;
    virtual uint getMinHeight(In<std::optional<uint>>) const override;

    virtual std::string_view getWidgetTypeName() const override { return WIDGET_TYPE_NAME; }

    bool isHovered() const { return m_handle.isHovered(); }
    bool isSelected() const { return m_handle.isSelected(); }

    GroupWidget& getFirstChildGroup() { return m_first_group; }
    GroupWidget& getSecondChildGroup() { return m_second_group; }

    bool isHorizontal() const { return m_horizontal; }
    int  getSeperatorPos() const { return m_seperator_pos; }
    void setSeperatorPos(int i);

  protected:
    void onSizeChanged(In<ivec2>, In<ivec2>);

    void onMoveSeperator(In<ivec2> cursor, InOut<ivec2> movement);

    void updateLayout();

  private:
    GroupWidget m_first_group{*this};
    GroupWidget m_second_group{*this};

    InteractionHandle m_handle;

    bool m_horizontal;
    int  m_seperator_pos = 0;
};

export class SimpleSplitGroupWidgetView : public Widget::View {
  public:
    SimpleSplitGroupWidgetView(SplitGroupWidget& widget) : m_widget{widget} {}
    ~SimpleSplitGroupWidgetView() = default;

    void draw(RenderBatchInterface& batch) override;

  private:
    SplitGroupWidget& m_widget;
};

} // namespace bembel::gui
