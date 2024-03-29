module;
#include <string>
export module bembel.gui.widgets:TextInput;

import bembel.base;
import bembel.kernel;
import bembel.gui.core;

namespace bembel::gui {
using namespace bembel::base;
using namespace bembel::kernel;

export class TextInputWidget : public Widget {
  public:
    static constexpr std::string_view WIDGET_TYPE_NAME = "TextInput";

  public:
    TextInputWidget(In<Widget*> parent);
    ~TextInputWidget() = default;

    virtual bool configure(xml::Element const* properties) override;

    virtual uint getMinWidth(In<std::optional<uint>>) const override;
    virtual uint getMinHeight(In<std::optional<uint>>) const override;

    virtual std::string_view getWidgetTypeName() const override { return WIDGET_TYPE_NAME; }

    bool isDisabled() const { return m_handle.isDisabled(); }
    bool isHovered() const { return m_handle.isHovered(); }
    bool isSelected() const { return m_handle.isSelected(); }

    ObservableValue<std::u8string> text;

  protected:
    void onSizeChanged(In<ivec2>, In<ivec2>);
    void onAction(InteractionHandle::Action, ivec2);
    void onTextInput(char32_t);
    void onTextChanged(In<std::u8string>, In<std::u8string>);

    void copy();
    void paste();

  private:
    friend class View;
    class View : public Widget::View {
      public:
        View(TextInputWidget* widget) : m_widget{widget} {}
        ~View() = default;

        void draw(InOut<RenderBatchInterface> batch) override;

        void updateGlyphs(In<std::u8string> str);

      protected:
        virtual void drawBackground(
            InOut<RenderBatchInterface> batch, In<Style> style, In<vec2> min, In<vec2> max
        );
        virtual void drawText(
            InOut<RenderBatchInterface> batch,
            In<Style>                   style,
            In<SdfFont>                 font,
            In<vec2>                    origin,
            In<float>                   scale
        );
        virtual void drawCursor(
            InOut<RenderBatchInterface> batch,
            In<Style>                   style,
            In<SdfFont>                 font,
            In<vec2>                    text_origin,
            In<float>                   text_scale,
            In<usize>                   cursor_pos
        );

      protected:
        TextInputWidget*                          m_widget;
        std::vector<std::pair<GlyphIndex, float>> m_glyphs;
        float                                     m_advance = 0;
    };

  private:
    InteractionHandle m_handle;

    usize m_cursor_pos = 0;
};

} // namespace bembel::gui