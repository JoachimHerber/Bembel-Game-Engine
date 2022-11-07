module;
#include "bembel/pch.h"
export module bembel.gui.widgets:Button;

import bembel.base;
import bembel.kernel;
import bembel.gui.core;

namespace bembel::gui {
using namespace bembel::base;
using namespace bembel::kernel;

export class ButtonWidget : public Widget {
  public:
    static constexpr std::string_view WIDGET_TYPE_NAME = "Button";

  public:
    ButtonWidget(Widget& parent);
    ~ButtonWidget();

    virtual bool configure(xml::Element const* properties) override;

    virtual std::string_view getWidgetTypeName() const override { return WIDGET_TYPE_NAME; }

    bool isDisabled() const { return m_handle.isDisabled(); }
    bool isHovered() const { return m_handle.isHovered(); }
    bool isSelected() const { return m_handle.isSelected(); }
    bool isPressed() const { return m_is_pressed; }

    Signal<> press_signal;
    Signal<> release_signal;
    Signal<> click_signal;

  protected:
    void onSizeChanged(In<ivec2>, In<ivec2>);

    void onPress(In<ivec2>);
    void onRelease(In<ivec2>);

  private:
    InteractionHandle m_handle;

    bool m_is_pressed = false;
};

export class SimpleButtonWidgetView : public Widget::View {
  public:
    SimpleButtonWidgetView(ButtonWidget& button) : m_button{button} {}
    ~SimpleButtonWidgetView() = default;

    void draw(RenderBatchInterface& batch) override;

  private:
    ButtonWidget& m_button;
};

} // namespace bembel::gui
