module;
export module bembel.gui.core:GraphicalUserInterface;

import bembel.base;
import bembel.kernel;
import :RenderBatch;
import :InteractionHandle;
import :GroupWidget;
import :InputHandler;
import :Renderer;

namespace bembel::gui {
using namespace bembel::base;
using namespace bembel::kernel;

export class GraphicalUserInterface {
  public:
    class View : public kernel::Viewport::View {
      public:
        View(In<GraphicalUserInterface*> gui) : m_gui{gui} {}

        void onCurserMove(In<vec2> pos) override;
        void onResize(In<uvec2>) override;

        void draw(In<ivec2> pos, In<uvec2> size) override;

      private:
        GraphicalUserInterface* m_gui;
    };

    GraphicalUserInterface(In<Engine*> engine);
    GraphicalUserInterface(GraphicalUserInterface const&)            = delete;
    GraphicalUserInterface& operator=(GraphicalUserInterface const&) = delete;
    ~GraphicalUserInterface();

    bool init(xml::Element const*, bool load_file = true);
    bool init(std::filesystem::path file_name);

    template <typename WidgetType>
    WidgetType* getWidget(std::string_view path) const;
    Widget*     getWidget(std::string_view path) const;

  public:
    Engine*      engine;
    View         view;
    InputHandler input;
    Renderer     renderer;
    GroupWidget  root_widget;

  private:
    friend class Widget;
};

template <typename WidgetType>
inline WidgetType* GraphicalUserInterface::getWidget(std::string_view path) const {
    auto widget = this->getWidget(path);
    if(widget != nullptr && widget->getWidgetTypeName() == WidgetType::WIDGET_TYPE_NAME) {
        return static_cast<WidgetType*>(widget);
    }
    return nullptr;
}
} // namespace bembel::gui
