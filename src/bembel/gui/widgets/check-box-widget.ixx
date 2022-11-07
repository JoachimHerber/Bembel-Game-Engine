module;
#include "bembel/pch.h"
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
    CheckBoxWidget(Widget& parent);
    ~CheckBoxWidget() = default;

    virtual bool configure(xml::Element const* properties) override;

    virtual std::string_view getWidgetTypeName() const override { return WIDGET_TYPE_NAME; }

    virtual ivec2 getMinSize() const override;

    void disable() {
        m_handle.disable();
        m_label.text_color = ColorRGBA(127, 127, 127, 255);
        m_label.outline    = false;
    }
    void enable() {
        m_handle.enable();
        m_label.text_color.reset();
        m_label.outline = true;
    }
    bool isDisabled() const { return m_handle.isDisabled(); }

    enum class State { UNSELECTED = 0, SELECTED = 1, INDETERMINATE = -1 };

    ObservableValue<State>        state   = State::UNSELECTED;
    ObservableValue<std::string>& text    = m_label.text;
    ObservableValue<bool>&        outline = m_label.outline;

  protected:
    void onSizeChanged(In<ivec2>, In<ivec2>);

    void onAction(InteractionHandle::Action, ivec2);

  private:
    LabelWidget m_label{*this};

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