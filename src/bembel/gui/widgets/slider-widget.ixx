module;
#include <limits>
#include <string>
export module bembel.gui.widgets:Slider;

import bembel.base;
import bembel.kernel;
import bembel.gui.core;
import :Label;

namespace bembel::gui {
using namespace bembel::base;
using namespace bembel::kernel;

export class SliderWidget : public Widget {
  public:
    static constexpr std::string_view WIDGET_TYPE_NAME = "Slider";

  public:
    SliderWidget(Widget& parent);
    ~SliderWidget();

    virtual bool configure(xml::Element const* properties) override;

    virtual uint getMinWidth() const override;
    virtual uint getMinHeight() const override;

    virtual std::string_view getWidgetTypeName() const override { return WIDGET_TYPE_NAME; }

    void disable() { m_handle.disable(); }
    void enable() { m_handle.enable(); }

    bool isDisabled() const { return m_handle.isDisabled(); }
    bool isHovered() const { return m_handle.isHovered(); }
    bool isSelected() const { return m_handle.isSelected(); }
    bool isActive() const { return m_active; }

    double getSliderPosition() { return m_position; }

    Signal<InOut<double>> position_constrain_signal;

  protected:
    void onSizeChanged(In<ivec2>, In<ivec2>);

    void onHandlePress(In<ivec2> cursor);
    void onHandleRelease(In<ivec2> cursor);
    void onHandleMoved(In<ivec2> cursor, InOut<ivec2> movement);

    void updateSliderPos(In<ivec2> cursor_pos);

  protected:
    double m_position = 0.0;

  private:
    InteractionHandle m_handle;

    bool m_active = false;
};

export class SliderWidgetView : public Widget::View {
  public:
    SliderWidgetView(SliderWidget& slider) : m_slider{slider} {}
    ~SliderWidgetView() = default;

    void draw(RenderBatchInterface& batch) override;

  private:
    SliderWidget& m_slider;
};

export class IntSliderWidget : public SliderWidget {
  public:
    static constexpr std::string_view WIDGET_TYPE_NAME = "IntSlider";

  public:
    IntSliderWidget(
        Widget& parent,
        i64     min         = std::numeric_limits<i64>::lowest(),
        i64     max         = std::numeric_limits<i64>::max(),
        bool    logarithmic = false
    );
    ~IntSliderWidget() = default;

    virtual bool configure(xml::Element const* properties) override;

    virtual std::string_view getWidgetTypeName() const override { return WIDGET_TYPE_NAME; }

    void setValue(i64 value);
    i64  getValue() const { return m_value; }

    Signal<In<i64>> value_update_signal;

  protected:
    void onSizeChanged(In<ivec2>, In<ivec2> new_size) { m_label.size = new_size; }
    void constrainSliderPos(InOut<double> pos);

    void updateLabel();

  private:
    LabelWidget m_label;

    i64 m_min;
    i64 m_max;

    bool               m_logarithmic;
    std::optional<i64> m_step;

    String m_unit;
    String m_prefix;

    i64 m_value;
};

} // namespace bembel::gui
