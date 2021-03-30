#ifndef BEMBEL_GUI_GRAPHICAL_USER_INTERFACE_HPP
#define BEMBEL_GUI_GRAPHICAL_USER_INTERFACE_HPP

#include <bembel/base/utils/factory.hpp>
#include <bembel/kernel/display/viewport.hpp>
#include <bembel/kernel/events/display-events.hpp>
#include <bembel/kernel/events/event-manager.hpp>

#include "./interaction/input-handler.hpp"
#include "./rendering/renderer.hpp"
#include "./widgets/widget.hpp"

namespace bembel::gui {
class GroupWidget;

class BEMBEL_API GraphicalUserInterface {
  public:
    class View : public kernel::Viewport::View {
      public:
        View(GraphicalUserInterface* gui)
        : gui{gui} {}

        void onCurserMove(const glm::vec2& pos) override;
        void onResize(const glm::uvec2&) override;

        void draw(const glm::ivec2& pos, const glm::uvec2& size) override;

      private:
        GraphicalUserInterface* gui;
    };

    GraphicalUserInterface(kernel::EventManager& event_mgr, kernel::AssetManager& asset_mgr);
    GraphicalUserInterface(const GraphicalUserInterface&) = delete;
    GraphicalUserInterface& operator=(const GraphicalUserInterface&) = delete;
    ~GraphicalUserInterface();

    bool init(const base::xml::Element*, bool load_file = true);
    bool init(const std::string& file_name);

    Widget* getWidget(const std::string& widget_name) const;
    template <typename WidgetType>
    WidgetType* getWidget(const std::string& widget_name) const;

    Renderer&     getRenderer();
    InputHandler& getInputHandler();

    GroupWidget* getRootWidget() const;

    kernel::EventManager& getEventManager();
    kernel::AssetManager& getAssetManager();

    View* getView() { return &(this->view); }

  private:
    bool initWidgets(const base::xml::Element*);

  private:
    kernel::EventManager& event_mgr;
    kernel::AssetManager& asset_mgr;

    View view;

    Renderer     renderer;
    InputHandler input_handler;

    std::unique_ptr<GroupWidget> root_widget;

    std::map<std::string, Widget*> named_widgets;

    friend class Widget;
};

template <typename WidgetType>
inline WidgetType* GraphicalUserInterface::getWidget(const std::string& widgetName) const {
    auto widget = this->getWidget(widgetName);
    if(widget != nullptr && widget->getWidgetTypeName() == WidgetType::WIDGET_TYPE_NAME) {
        return dynamic_cast<WidgetType*>(widget);
    }
    return nullptr;
}
} // namespace bembel::gui
#endif // include guards
