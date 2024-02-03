module;
#include <string_view>
export module bembel.gui.widgets:RadioButton;

import bembel.base;
import bembel.kernel;
import bembel.gui.core;
import :Label;

namespace bembel::gui {
using namespace bembel::base;
using namespace bembel::kernel;

export class RadioButtonWidget : public Widget {
  public:
    static constexpr std::string_view WIDGET_TYPE_NAME = "RadioButton";

  public:
    RadioButtonWidget(In<Widget*> parent, int index);
    ~RadioButtonWidget() = default;

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
    bool isFocused() const { return m_handle.isSelected(); }

    Signal<int> select_signal;

    void select() {
        if(!m_selected) select_signal.emit(m_index);
        m_selected = true;
    }

    void deselect() { m_selected = false; }

    bool isSelected() const { return m_selected; }

    std::u8string_view getText() const { return m_label.getText(); }
    void               setText(std::u8string_view text) { m_label.setText(text); }

  protected:
    void onSizeChanged(In<ivec2>, In<ivec2>);

    void onAction(InteractionHandle::Action, ivec2);

  private:
    LabelWidget m_label{this};

    int  m_index;
    bool m_selected = false;

    InteractionHandle m_handle;
};

export class SimpleRadioButtonWidgetView : public Widget::View {
  public:
    SimpleRadioButtonWidgetView(RadioButtonWidget* widget) : m_widget{widget} {}

    ~SimpleRadioButtonWidgetView() = default;

    void draw(InOut<RenderBatchInterface> batch) override;

  private:
    RadioButtonWidget* m_widget;
};

export class RadioButtonGroupWidget : public Widget {
  public:
    static constexpr std::string_view WIDGET_TYPE_NAME = "RadioButtonGroup";

  public:
    RadioButtonGroupWidget(In<Widget*> parent);
    ~RadioButtonGroupWidget() = default;

    virtual bool configure(xml::Element const* properties) override;

    virtual uint getMinWidth(In<std::optional<uint>>) const override;
    virtual uint getMinHeight(In<std::optional<uint>>) const override;

    virtual std::string_view getWidgetTypeName() const override { return WIDGET_TYPE_NAME; }

    void disableButton(size_t index);
    void enableButton(size_t index);

    void deleteAllRadioButtons();
    void addRadioButton(In<std::u8string_view> lable);

    int  getSelection() const;
    void setSelection(int index);

    Signal<int> selection_change_signal;

  protected:
    void onSizeChanged(In<ivec2>, In<ivec2>);

  private:
    std::vector<std::unique_ptr<RadioButtonWidget>> m_buttons;

    int m_selection = -1;
};

} // namespace bembel::gui