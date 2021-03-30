#ifndef BEMBEL_GUI_RENDERER_WIDGETVIEW_HPP
#define BEMBEL_GUI_RENDERER_WIDGETVIEW_HPP

#include "./render-batch.hpp"
#include "./style.hpp"

namespace bembel::gui {

class BEMBEL_API WidgetView {
  public:
    WidgetView(){};
    virtual ~WidgetView(){};

    virtual void draw(RenderBatchInterface& batch) = 0;
};

} // namespace bembel::gui
#endif // include guards
