#ifndef BEMBEL_GUI_WIDGETS_WIDGET_HPP
#define BEMBEL_GUI_WIDGETS_WIDGET_HPP

#include <bembel/base/io/xml.hpp>
#include <bembel/kernel/core/system.hpp>
#include <bembel/kernel/events/display-events.hpp>

#include "../interaction/interaction-handle.hpp"
#include "../interaction/observable-value.hpp"
#include "../rendering/style.hpp"
#include "../rendering/widget-view.hpp"

namespace bembel::gui {
class GraphicalUserInterface;

class BEMBEL_API Widget {
  public:
    Widget(GraphicalUserInterface* gui, Widget* parent);
    Widget(const Widget&) = delete;
    Widget& operator=(const Widget&) = delete;
    virtual ~Widget();

    virtual bool init()                                     = 0;
    virtual bool init(const base::xml::Element* properties) = 0;

    virtual glm::ivec2 getMinSize() const;
    virtual glm::ivec2 getMaxSize() const;

    WidgetView* getView();

    const std::string& getName() const { return this->name; }
    bool               setName(const std::string& name);

    ObservableValue<glm::ivec2> position;
    ObservableValue<glm::ivec2> size;

    GraphicalUserInterface* getGUI() const;
    Style*                  getStyle() const;

    const Widget*                          getParentWidget() const;
    const std::vector<Widget*>&            getChildWidgets() const;
    const std::vector<InteractionHandle*>& getInteractionHandles() const;

    virtual const std::string& getWidgetTypeName() const = 0;

  protected:
    std::string name;

    GraphicalUserInterface* gui;

    std::unique_ptr<WidgetView> view;

    Widget*                         parent;
    std::vector<Widget*>            child_widgets;
    std::vector<InteractionHandle*> interaction_handles;
};

} // namespace bembel::gui
#endif // include guards
