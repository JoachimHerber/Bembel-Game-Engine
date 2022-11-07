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

    GraphicalUserInterface(EventManager& event_mgr, AssetManager& asset_mgr);
    GraphicalUserInterface(GraphicalUserInterface const&)            = delete;
    GraphicalUserInterface& operator=(GraphicalUserInterface const&) = delete;
    ~GraphicalUserInterface();

    bool init(xml::Element const*, bool load_file = true);
    bool init(std::filesystem::path file_name);

    template <typename WidgetType>
    WidgetType* getWidget(std::string_view path) const;
    Widget* getWidget(std::string_view path) const { return m_root_widget.getChildWidget(path); }

    EventManager& getEventManager() /**/ { return m_event_mgr; }
    AssetManager& getAssetManager() /**/ { return m_asset_mgr; }
    View&         getView() /**********/ { return m_view; }
    InputHandler& getInputHandler() /**/ { return m_input_handler; }
    Renderer&     getRenderer() /******/ { return m_renderer; }
    GroupWidget&  getRootWidget() /****/ { return m_root_widget; }

  private:
    EventManager& m_event_mgr;
    AssetManager& m_asset_mgr;

    View m_view{*this};

    InputHandler m_input_handler;
    Renderer     m_renderer;

    GroupWidget m_root_widget;

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
