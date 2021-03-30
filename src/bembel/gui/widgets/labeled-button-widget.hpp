#ifndef BEMBEL_GUI_WIDGETS_LABELED_BUTTON_WIDGET_HPP
#define BEMBEL_GUI_WIDGETS_LABELED_BUTTON_WIDGET_HPP

#include "./button-widget.hpp"
#include "./label-widget.hpp"
#include "./widget.hpp"

namespace bembel::gui {

class BEMBEL_API LabeledButtonWidget : public Widget {
  public:
    LabeledButtonWidget(GraphicalUserInterface* gui, Widget* parent);
    ~LabeledButtonWidget();

    virtual bool init() override;
    virtual bool init(const base::xml::Element* properties) override;

    InteractionHandle::State getState();

    base::Signal<>& press_signal;
    base::Signal<>& release_signal;
    base::Signal<>& click_signal;

    ObservableValue<std::string>& text;

    static const std::string WIDGET_TYPE_NAME;
    const std::string&       getWidgetTypeName() const override;

  protected:
    void onSizeChanged(const glm::ivec2&, const glm::ivec2&);

  private:
    ButtonWidget button;
    LabelWidget  label;
};

} // namespace bembel::gui
#endif // include guards
