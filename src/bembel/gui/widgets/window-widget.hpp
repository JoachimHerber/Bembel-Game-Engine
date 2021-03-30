#ifndef BEMBEL_GUI_WIDGETS_WINDOW_WIDGET_HPP
#define BEMBEL_GUI_WIDGETS_WINDOW_WIDGET_HPP

#include "./button-widget.hpp"
#include "./group-widget.hpp"
#include "./widget.hpp"

namespace bembel::gui {

class BEMBEL_API WindowWidget : public Widget {
  public:
    WindowWidget(GraphicalUserInterface* gui, Widget* parent);
    ~WindowWidget();

    virtual bool init() override;
    virtual bool init(const base::xml::Element* properties) override;

    GroupWidget* getTitleBar();
    GroupWidget* getWindowArea();

    const static std::string WIDGET_TYPE_NAME;
    const std::string&       getWidgetTypeName() const override;

  protected:
    void onSizeChanged(const glm::ivec2&, const glm::ivec2&);

    void moveWidget(glm::ivec2&);

    void onLeftResizeHandleMoved(glm::ivec2&);
    void onBottomLeftResizeHandleMoved(glm::ivec2&);
    void onBottomResizeHandleMoved(glm::ivec2&);
    void onBottomRightResizeHandleMoved(glm::ivec2&);
    void onRightResizeHandleMoved(glm::ivec2&);

    void updateLayout();

  private:
    std::unique_ptr<GroupWidget> title_bar;
    std::unique_ptr<GroupWidget> window_area;

    InteractionHandle top_handle;
    // the resize hadels in ccw order
    // left, bottom-left, bottom-center, bottom-right, right
    std::array<InteractionHandle, 5> resize_handles;
};

class BEMBEL_API SimpleWindowWidgetView : public WidgetView {
  public:
    SimpleWindowWidgetView(WindowWidget* window);
    ~SimpleWindowWidgetView() = default;

    void draw(RenderBatchInterface& batch) override;

  private:
    WindowWidget* window;
};

} // namespace bembel::gui
#endif // include guards
