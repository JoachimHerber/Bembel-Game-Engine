module;
#include <string>
export module bembel.gui.widgets:Window;

import bembel.base;
import bembel.kernel;
import bembel.gui.core;

namespace bembel::gui {
using namespace bembel::base;
using namespace bembel::kernel;

export class WindowWidget : public Widget {
  public:
    static constexpr std::string_view WIDGET_TYPE_NAME = "Window";

  public:
    WindowWidget(Widget& parent);
    ~WindowWidget();

    virtual bool configure(xml::Element const* properties) override;

    virtual uint getMinWidth() const override;
    virtual uint getMinHeight() const override;

    virtual std::string_view getWidgetTypeName() const override { return WIDGET_TYPE_NAME; }

    GroupWidget& getTitleBar() { return m_title_bar; }
    GroupWidget& getWindowArea() { return m_window_area; }

  protected:
    void onSizeChanged(In<ivec2>, In<ivec2>);

    void moveWidget(In<ivec2> cursor, InOut<ivec2> movement);

    void onLeftResizeHandleMoved /*********/ (In<ivec2> cursor, InOut<ivec2> movement);
    void onBottomLeftResizeHandleMoved /***/ (In<ivec2> cursor, InOut<ivec2> movement);
    void onBottomResizeHandleMoved /*******/ (In<ivec2> cursor, InOut<ivec2> movement);
    void onBottomRightResizeHandleMoved /**/ (In<ivec2> cursor, InOut<ivec2> movement);
    void onRightResizeHandleMoved /********/ (In<ivec2> cursor, InOut<ivec2> movement);

    void updateLayout();

  private:
    GroupWidget m_title_bar{*this};
    GroupWidget m_window_area{*this};

    InteractionHandle m_top_handle;
    InteractionHandle m_resize_handle_left;
    InteractionHandle m_resize_handle_bottom_left;
    InteractionHandle m_resize_handle_bottom_center;
    InteractionHandle m_resize_handle_bottom_right;
    InteractionHandle m_resize_handle_right;
};

export class SimpleWindowWidgetView : public Widget::View {
  public:
    SimpleWindowWidgetView(WindowWidget& window) : m_window{window} {}
    ~SimpleWindowWidgetView() = default;

    void draw(RenderBatchInterface& batch) override;

  private:
    WindowWidget& m_window;
};

} // namespace bembel::gui
