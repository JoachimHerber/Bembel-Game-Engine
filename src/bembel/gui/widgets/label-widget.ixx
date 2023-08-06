module;
#include <string>
#include <optional>
export module bembel.gui.widgets:Label;

import bembel.base;
import bembel.kernel;
import bembel.gui.core;

namespace bembel::gui {
using namespace bembel::base;
using namespace bembel::kernel;

export class LabelWidget : public Widget {
  public:
    static constexpr std::string_view WIDGET_TYPE_NAME = "Label";

  public:
    LabelWidget(Widget& parent, std::u8string_view text = {u8""});
    ~LabelWidget() = default;

    virtual bool configure(xml::Element const* properties) override;

    virtual uint getMinWidth() const override;
    virtual uint getMinHeight() const override;

    virtual std::string_view getWidgetTypeName() const override { return WIDGET_TYPE_NAME; }

    enum class Alignment { Left, Center, Right };

    std::u8string_view       getText() const { return m_text; }
    std::optional<ColorRGBA> getTextColor() const { return m_text_color; }
    Alignment                getAlignment() const { return m_alignment; }
    bool                     getHasOutline() const { return m_outline; }

    void setText(std::u8string_view text) {
        m_text = text;
        updateGlyphs();
    }
    void setTextColor(std::optional<ColorRGBA> color) { m_text_color = color; }
    void setAlignment(Alignment alignment) { m_alignment = alignment; }
    void setHasOutline(bool outline) { m_outline = outline; }

  private:
    void updateGlyphs();

  private:
    friend class View;

    class View : public Widget::View {
      public:
        View(LabelWidget& label) : m_label{label} {}

        ~View() = default;

        void draw(RenderBatchInterface& batch) override;

      private:
        LabelWidget& m_label;
    };
    struct Glyph {
        unsigned index;
        float    x;
    };

  private:
    std::u8string            m_text;
    std::optional<ColorRGBA> m_text_color;
    Alignment                m_alignment = Alignment::Left;
    bool                     m_outline   = false;

    std::vector<Glyph> m_glyphs;
    float              m_text_length = 0.0f;
};

} // namespace bembel::gui
