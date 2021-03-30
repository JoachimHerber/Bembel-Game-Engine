#ifndef BEMBEL_GUI_WIDGETS_WIDGETDEBUGVIEW_HPP
#define BEMBEL_GUI_WIDGETS_WIDGETDEBUGVIEW_HPP

#include "../rendering/widget-view.hpp"
#include "./widget.hpp"

namespace bembel::gui {

class BEMBEL_API WidgetDebugView : public WidgetView {
  public:
    WidgetDebugView(Widget* widget);
    ~WidgetDebugView();

    virtual void render(RenderBatch& batch) override;

  private:
    Widget* widget;
};

} // namespace bembel::gui
#endif // include guards
