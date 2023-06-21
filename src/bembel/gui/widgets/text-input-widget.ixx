module;
#include <string>
export module bembel.gui.widgets:TextInput;

import bembel.base;
import bembel.kernel;
import bembel.text;
import bembel.gui.core;

namespace bembel::gui {
using namespace bembel::base;
using namespace bembel::kernel;
using namespace bembel::text;

export class TextInputWidget : public Widget {
  public:
    static constexpr std::string_view WIDGET_TYPE_NAME = "TextInput";

  public:
    TextInputWidget(Widget& parent);
    ~TextInputWidget() = default;

    virtual bool configure(xml::Element const* properties) override;

    virtual uint getMinWidth() const override;
    virtual uint getMinHeight() const override;

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
        View(TextInputWidget& widget) : m_widget{widget} {}
        ~View() = default;

        void draw(RenderBatchInterface& batch) override;

        void updateGlyphs(In<std::u8string> str);

      protected:
        virtual void drawBackground(
            RenderBatchInterface& batch, Style const& style, vec2 min, vec2 max
        );
        virtual void drawText(
            RenderBatchInterface& batch,
            Style const&          style,
            Font const&           font,
            vec2                  origin,
            float                 scale
        );
        virtual void drawCursor(
            RenderBatchInterface& batch,
            Style const&          style,
            Font const&           font,
            vec2                  text_origin,
            float                 text_scale,
            usize                 cursor_pos
        );

      protected:
        TextInputWidget&                          m_widget;
        std::vector<std::pair<GlyphIndex, float>> m_glyphs;
        float                                     m_advance = 0;
    };

  private:
    InteractionHandle m_handle;

    usize m_cursor_pos = 0;
};

} // namespace bembel::gui