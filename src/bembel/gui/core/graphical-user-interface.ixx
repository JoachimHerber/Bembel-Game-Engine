module;
#include "bembel/pch.h"
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
        View(GraphicalUserInterface& gui) : m_gui{gui} {}

        void onCurserMove(vec2 const& pos) override;
        void onResize(uvec2 const&) override;

        void draw(ivec2 const& pos, uvec2 const& size) override;

      private:
        GraphicalUserInterface& m_gui;
    };

    GraphicalUserInterface(AssetManager& asset_mgr);
    GraphicalUserInterface(GraphicalUserInterface const&)            = delete;
    GraphicalUserInterface& operator=(GraphicalUserInterface const&) = delete;
    ~GraphicalUserInterface();

    bool init(xml::Element const*, bool load_file = true);
    bool init(std::filesystem::path file_name);

    template <typename WidgetType>
    WidgetType* getWidget(std::string_view path) const;
    Widget*     getWidget(std::string_view path) const;

  public:
    AssetManager& assets;

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
