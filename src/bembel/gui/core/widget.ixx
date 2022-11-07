module;
#include "bembel/pch.h"
export module bembel.gui.core:Widget;

import bembel.base;
import bembel.kernel;
import :Style;
import :RenderBatch;
import :InteractionHandle;
import :WidgetLayoutParams;

namespace bembel::gui {
using namespace bembel::base;
using namespace bembel::kernel;

export class GraphicalUserInterface;

export class Widget {
  public:
    class View {
      public:
        View(){};
        virtual ~View(){};

        virtual void draw(RenderBatchInterface& batch) = 0;
    };

  public:
    Widget(GraphicalUserInterface& gui) : m_gui{gui}, m_parent{nullptr} {}
    Widget(Widget& parent) : m_gui{parent.getGUI()}, m_parent{&parent} {}
    Widget(Widget const&)            = delete;
    Widget& operator=(Widget const&) = delete;
    virtual ~Widget()                = default;

    virtual bool configure(xml::Element const* properties);

    virtual ivec2 getMinSize() const { return {0, 0}; }
    virtual ivec2 getMaxSize() const {
        return {std::numeric_limits<int>::max(), std::numeric_limits<int>::max()};
    }

    virtual std::string_view getWidgetTypeName() const = 0;

    View* getView() { return m_view.get(); }

    bool isHidden() const { return m_hidden; }
    void show();
    void hide();

    std::string const& getName() const { return m_name; }
    void               setName(std::string_view name) { m_name = name; }

    WidgetLayoutParams const& getLayoutParams() const { return m_layout_params; }
    void setLayoutParams(In<WidgetLayoutParams>& param) { m_layout_params = param; }

    ObservableValue<ivec2> position = {0, 0};
    ObservableValue<ivec2> size     = {1, 1};

    GraphicalUserInterface& getGUI() const { return m_gui; }
    Style*                  getStyle() const;

    Widget*                                getParentWidget() const { return m_parent; }
    std::vector<Widget*> const&            getChildWidgets() const { return m_child_widgets; }
    std::vector<InteractionHandle*> const& getInteractionHandles() const {
        return m_interaction_handles;
    }

    Widget* getChildWidget(std::string_view path) const;

    static Factory<Widget, Widget&>& getFactory();

  protected:
    GraphicalUserInterface& m_gui;
    Widget*                 m_parent;

    std::string m_name;

    std::unique_ptr<View> m_view;
    bool                  m_hidden = false;

    std::vector<Widget*>            m_child_widgets;
    std::vector<InteractionHandle*> m_interaction_handles;

    WidgetLayoutParams m_layout_params;
};

} // namespace bembel::gui
