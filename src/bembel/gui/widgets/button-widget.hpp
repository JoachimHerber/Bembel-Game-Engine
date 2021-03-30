#ifndef BEMBEL_GUI_WIDGETS_BUTTON_WIDGET_HPP
#define BEMBEL_GUI_WIDGETS_BUTTON_WIDGET_HPP

#include "./widget.hpp"

namespace bembel::gui {

class BEMBEL_API ButtonWidget : public Widget {
  public:
    ButtonWidget(GraphicalUserInterface* gui, Widget* parent);
    ~ButtonWidget();

    virtual bool init() override;
    virtual bool init(const base::xml::Element* properties) override;

    InteractionHandle::State getState();

    base::Signal<> press_signal;
    base::Signal<> release_signal;
    base::Signal<> click_signal;

    static const std::string WIDGET_TYPE_NAME;
    const std::string&       getWidgetTypeName() const override;

  protected:
    void onSizeChanged(const glm::ivec2&, const glm::ivec2&);

    void onStateChange(InteractionHandle::State, InteractionHandle::State);

  private:
    InteractionHandle handle;
};

class SimpleButtonWidgetView : public WidgetView {
  public:
    SimpleButtonWidgetView(ButtonWidget* button);
    ~SimpleButtonWidgetView();

    void draw(RenderBatchInterface& batch) override;

  private:
    ButtonWidget* button;
};

} // namespace bembel::gui
#endif // include guards
