#ifndef BEMBEL_GUI_WIDGET_GROUPWIDGET_HPP
#define BEMBEL_GUI_WIDGET_GROUPWIDGET_HPP

#include <bembel/base/utils/factory.hpp>

#include "./widget.hpp"

namespace bembel::gui {

class BEMBEL_API WidgetLayouter {
  public:
    using WidgetIterator      = std::vector<Widget*>::iterator;
    using ConstWidgetIterator = std::vector<Widget*>::const_iterator;

    virtual void calculateLayout(const glm::ivec2&, WidgetIterator, WidgetIterator) = 0;

    virtual glm::ivec2 calculateMinSize(ConstWidgetIterator, ConstWidgetIterator) = 0;
};

class BEMBEL_API GroupWidget : public Widget {
  public:
    GroupWidget(GraphicalUserInterface* _gui, Widget* _parent);
    ~GroupWidget();

    virtual bool init() override;
    virtual bool init(const base::xml::Element* properties) override;

    template <typename WidgetType>
    WidgetType* createChildWidget();
    Widget*     createChildWidget(const std::string& widget_type_name);

    void setLayouter(std::shared_ptr<WidgetLayouter>);

    glm::ivec2 getMinSize() const override;

    const static std::string WIDGET_TYPE_NAME;
    const std::string&       getWidgetTypeName() const override;

  private:
    void onSizeChanged(const glm::ivec2&, const glm::ivec2&);

  private:
    std::vector<std::unique_ptr<Widget>> widgets;

    std::shared_ptr<WidgetLayouter> layouter;
};

class BEMBEL_API LinearWidgetLayouter : public WidgetLayouter {
  public:
    enum class Direction { VERTICAL, HORIZONTAL };
    enum class Alignment {
        LEFT   = 0,
        BUTTON = 0,
        CENTER = 1,
        RIGHT  = 2,
        TOP    = 2,
    };
    enum class ResizeMode { MIN, MAX, NO_RESIZE };

    LinearWidgetLayouter(const std::string&, const std::string&);
    LinearWidgetLayouter(Direction, Alignment);
    ~LinearWidgetLayouter();

    void calculateLayout(
        const glm::ivec2& area_size, WidgetLayouter::WidgetIterator begin, WidgetLayouter::WidgetIterator end) override;

    glm::ivec2 calculateMinSize(
        WidgetLayouter::ConstWidgetIterator begin, WidgetLayouter::ConstWidgetIterator end) override;

  private:
    Direction  direction   = Direction::VERTICAL;
    Alignment  alignment   = Alignment::CENTER;
    ResizeMode resize_mode = ResizeMode::NO_RESIZE;
};

template <typename WidgetType>
inline WidgetType* GroupWidget::createChildWidget() {
    auto widget     = std::make_unique<WidgetType>(this->gui, this);
    auto widget_ptr = widget.get();
    this->widgets.push_back(std::move(widget));
    this->child_widgets.push_back(widget_ptr);
    return widget_ptr;
}

} // namespace bembel::gui
#endif // include guards
