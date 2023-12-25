module;
#include <string_view>
export module bembel.gui.widgets:CheckBox;

import bembel.base;
import bembel.kernel;
import bembel.gui.core;
import :Label;

namespace bembel::gui {
using namespace bembel::base;
using namespace bembel::kernel;

export class CheckBoxWidget : public Widget {
  public:
    static constexpr std::string_view WIDGET_TYPE_NAME = "CheckBox";

  public:
    CheckBoxWidget(Widget& parent, std::u8string_view label = u8"");
    ~CheckBoxWidget() = default;

    virtual bool configure(xml::Element const* properties) override;

    virtual uint getMinWidth(In<std::optional<uint>>) const override;
    virtual uint getMinHeight(In<std::optional<uint>>) const override;

    virtual std::string_view getWidgetTypeName() const override { return WIDGET_TYPE_NAME; }

    void disable() {
        m_handle.disable();
        m_label.setTextColor({ColorRGBA(127, 127, 127, 255)});
    }
    void enable() {
        m_handle.enable();
        m_label.setTextColor({});
    }
    bool isDisabled() const { return m_handle.isDisabled(); }
    bool isHovered() const { return m_handle.isHovered(); }

    enum class State { UNSELECTED = 0, SELECTED = 1, INDETERMINATE = -1 };

    ObservableValue<State> state = State::UNSELECTED;

    bool isSelected() const { return state.get() == State::SELECTED; }

    std::u8string_view getText() const { return m_label.getText(); }
    void               setText(std::u8string_view text) { m_label.setText(text); }

  protected:
    void onSizeChanged(In<ivec2>, In<ivec2>);

    void onAction(InteractionHandle::Action, ivec2);

  private:
    LabelWidget m_label;

    InteractionHandle m_handle;
};

export class SimpleCheckBoxWidgetView : public Widget::View {
  public:
    SimpleCheckBoxWidgetView(CheckBoxWidget& widget) : m_widget{widget} {}
    ~SimpleCheckBoxWidgetView() = default;

    void draw(RenderBatchInterface& batch) override;

  private:
    CheckBoxWidget& m_widget;
};

} // namespace bembel::gui