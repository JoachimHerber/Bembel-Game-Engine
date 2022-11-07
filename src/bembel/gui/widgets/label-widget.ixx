module;
#include "bembel/pch.h"
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
    LabelWidget(Widget& parent);
    ~LabelWidget() = default;

    virtual bool configure(xml::Element const* properties) override;

    virtual std::string_view getWidgetTypeName() const override { return WIDGET_TYPE_NAME; }

    enum class Alignment { Left, Center, Right };

    ObservableValue<std::string> text;
    std::optional<ColorRGBA>     text_color;
    ObservableValue<Alignment>   alignment;
    ObservableValue<bool>        outline;

  private:
    void onTextChanged(std::string const&, std::string const&);

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
    std::vector<Glyph> m_glyphs;
    float              m_text_length = 0.0f;
};

} // namespace bembel::gui
