module;
#include <concepts>
#include <optional>
#include <span>
#include <vector>
export module bembel.gui.core:Widget;

import bembel.base;
import bembel.kernel;
import :Style;
import :RenderBatch;
import :InteractionHandle;

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

    class Layout {
      public:
        virtual bool configure(In<xml::Element const*> properties) = 0;

        virtual uint getMinWidth(In<std::optional<uint>> height = {}) const = 0;
        virtual uint getMinHeight(In<std::optional<uint>> width = {}) const = 0;

        virtual void updateLayout()              = 0;
        virtual void updateLayout(In<vec2> size) = 0;

        static Factory<Layout>& getLayouterFactory();
    };

  public:
    Widget(GraphicalUserInterface& gui) : m_gui{gui}, m_parent{nullptr} {}
    Widget(Widget& parent) : m_gui{parent.m_gui}, m_parent{&parent} {}
    Widget(Widget const&)            = delete;
    Widget& operator=(Widget const&) = delete;
    virtual ~Widget()                = default;

    virtual bool configure(xml::Element const* properties);

    virtual uint getMinWidth(In<std::optional<uint>> height = {}) const = 0;
    virtual uint getMinHeight(In<std::optional<uint>> width = {}) const = 0;

    virtual std::string_view getWidgetTypeName() const = 0;

    View* getView() const { return m_view.get(); }

    bool isHidden() const { return m_hidden; }
    void show();
    void hide();

    std::string const& getName() const { return m_name; }
    void               setName(std::string_view name) { m_name = name; }

    ObservableValue<ivec2> position = {0, 0};
    ObservableValue<ivec2> size     = {1, 1};

    GraphicalUserInterface& getGUI() const { return m_gui; }
    Style*                  getStyle() const;

    Widget*                                getParentWidget() const { return m_parent; }
    std::vector<Widget*> const&            getChildWidgets() const { return m_child_widgets; }
    std::vector<InteractionHandle*> const& getInteractionHandles() const {
        return m_interaction_handles;
    }

    template <typename WidgetType>
    WidgetType* getChildWidget(std::string_view path) const;
    Widget*     getChildWidget(std::string_view path) const;

    static Factory<Widget, Widget&>& getFactory();

  protected:
    GraphicalUserInterface& m_gui;
    Widget*                 m_parent;

    std::string m_name;

    std::unique_ptr<View> m_view;
    bool                  m_hidden     = false;

    std::vector<Widget*>            m_child_widgets;
    std::vector<InteractionHandle*> m_interaction_handles;
};

template <typename WidgetType>
inline WidgetType* Widget::getChildWidget(std::string_view path) const {
    auto widget = this->getChildWidget(path);
    if(widget != nullptr && widget->getWidgetTypeName() == WidgetType::WIDGET_TYPE_NAME) {
        return static_cast<WidgetType*>(widget);
    }
    return nullptr;
}
} // namespace bembel::gui
