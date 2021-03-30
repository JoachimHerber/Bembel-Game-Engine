#ifndef BEMBEL_GUI_WIDGETS_LABEL_WIDGET_HPP
#define BEMBEL_GUI_WIDGETS_LABEL_WIDGET_HPP

#include "./widget.hpp"

namespace bembel::gui {

class BEMBEL_API LabelWidget : public Widget {
  public:
    LabelWidget(GraphicalUserInterface* gui, Widget* parent);
    ~LabelWidget();

    virtual bool init() override;
    virtual bool init(const base::xml::Element* properties) override;

    enum class Alignment { Left, Center, Right };

    ObservableValue<std::string> text;
    ObservableValue<Alignment>   alignment;

    static const std::string WIDGET_TYPE_NAME;
    const std::string&       getWidgetTypeName() const override;

  private:
    void onTextChanged(const std::string&, const std::string&);

  private:
    friend class View;
    class View : public WidgetView {
      public:
        View(LabelWidget* label);
        ~View() = default;

        void draw(RenderBatchInterface& batch) override;

      private:
        LabelWidget* label;
    };

    struct Glyph {
        unsigned index;
        float    x;
    };

  private:
    std::vector<Glyph> glyphs;
    float              text_length = 0.0f;
};

} // namespace bembel::gui
#endif // include guards
