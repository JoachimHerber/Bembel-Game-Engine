module;
#include "bembel/pch.h"
export module bembel.gui.widgets:LabeledButton;

import bembel.base;
import bembel.kernel;
import bembel.gui.core;
import :Button;
import :Label;

namespace bembel::gui {
using namespace bembel::base;
using namespace bembel::kernel;

export class LabeledButtonWidget : public Widget {
  public:
    static constexpr std::string_view WIDGET_TYPE_NAME = "LabeledButton";

  public:
    LabeledButtonWidget(Widget& parent);

    virtual bool configure(xml::Element const* properties) override;

    virtual std::string_view getWidgetTypeName() const override { return WIDGET_TYPE_NAME; }

    bool isDisabled() const { return m_button.isDisabled(); }
    bool isHovered() const { return m_button.isHovered(); }
    bool isSelected() const { return m_button.isSelected(); }
    bool isPressed() const { return m_button.isPressed(); }

    Signal<>& press_signal   = m_button.press_signal;
    Signal<>& release_signal = m_button.release_signal;
    Signal<>& click_signal   = m_button.click_signal;

    ObservableValue<std::string>&            text      = m_label.text;
    ObservableValue<LabelWidget::Alignment>& alignment = m_label.alignment;
    ObservableValue<bool>&                   outline   = m_label.outline;

  protected:
    void onSizeChanged(In<ivec2>, In<ivec2>);

  private:
    ButtonWidget m_button{*this};
    LabelWidget  m_label{*this};
};

} // namespace bembel::gui
