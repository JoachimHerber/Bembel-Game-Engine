﻿module;
#include <optional>
#include <string>
export module bembel.gui.widgets:Text;

import bembel.base;
import bembel.kernel;
import bembel.gui.core;

namespace bembel::gui {
using namespace bembel::base;
using namespace bembel::kernel;

export class TextWidget : public Widget {
  public:
    static constexpr std::string_view WIDGET_TYPE_NAME = "Text";

  public:
    TextWidget(In<Widget*> parent);
    ~TextWidget() = default;

    virtual bool configure(xml::Element const* properties) override;

    virtual uint getMinWidth(In<std::optional<uint>>) const override;
    virtual uint getMinHeight(In<std::optional<uint>>) const override;

    virtual std::string_view getWidgetTypeName() const override { return WIDGET_TYPE_NAME; }

    ObservableValue<std::u8string> text;
    std::optional<ColorRGBA>       text_color;

  private:
    void onTextChanged(In<std::u8string>, In<std::u8string>);
    void onSizeChanged(In<ivec2>, In<ivec2>);

    void recalculateLayout(In<ivec2>);

  private:
    friend class View;

    class View : public Widget::View {
      public:
        View(TextWidget* widget) : m_widget{widget} {}

        ~View() = default;

        void draw(InOut<RenderBatchInterface> batch) override;

      private:
        TextWidget* m_widget;
    };

  private:
    Text       m_text;
    TextLayout m_layout;
    float      m_fontSize = 20;
};

} // namespace bembel::gui
