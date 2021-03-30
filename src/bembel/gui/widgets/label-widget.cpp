#include "./label-widget.hpp"

#include <bembel/base/utils/utils.hpp>
#include <bembel/kernel/assets/asset-manager.hpp>
#include <bembel/kernel/display/cursor-icon.hpp>
#include <bembel/kernel/rendering/text-layout.hpp>

#include "../graphical-user-interface.hpp"
#include "../rendering/style.hpp"
#include "./widget-type-utils.hpp"

namespace bembel::gui {

const std::string LabelWidget::WIDGET_TYPE_NAME = "Label";

namespace {
    BEMBEL_STATIC_INITIALIZATION {
        WidgetTypeUtils::registerWidgetType<LabelWidget>(LabelWidget::WIDGET_TYPE_NAME);
    }
} // namespace

LabelWidget::LabelWidget(GraphicalUserInterface* gui, Widget* parent)
: Widget{gui, parent}
, text{""}
, alignment{Alignment::Center} {
    this->text.change_signal.addSlot(this, &LabelWidget::onTextChanged);
}

LabelWidget::~LabelWidget() {
}

bool LabelWidget::init() {
    this->view = std::make_unique<LabelWidget::View>(this);
    return true;
}

bool LabelWidget::init(const base::xml::Element* properties) {
    glm::vec2 size;
    if(base::xml::getAttribute(properties, "size", size)) this->size.set(size);

    std::string text;
    if(base::xml::getAttribute(properties, "text", text)) this->text = text;

    std::string alignment = "";
    if(base::xml::getAttribute(properties, "alignment", alignment)) {
        std::transform(alignment.begin(), alignment.end(), alignment.begin(), [](char c) {
            return std::tolower(c);
        });
        if(alignment == "left") this->alignment = Alignment::Left;
        if(alignment == "right") this->alignment = Alignment::Right;
    }

    this->view = std::make_unique<LabelWidget::View>(this);
    return true;
}

const std::string& LabelWidget::getWidgetTypeName() const {
    return WIDGET_TYPE_NAME;
}

void LabelWidget::onTextChanged(const std::string&, const std::string& new_text) {
    auto style = this->getStyle();
    assert(style && "GUI::Style is undefined");
    auto font = style->getFont();
    assert(font && "Font is undefined");

    this->glyphs.clear();
    this->text_length   = 0;
    unsigned prev_index = kernel::Font::INVALIDE_GLYPH_INDEX;
    for(char32_t c : kernel::UTF8TextIterator(new_text)) {
        unsigned index = font->getGlyphIndex(c, false, false);
        if(index == kernel::Font::INVALIDE_GLYPH_INDEX) continue;

        if(prev_index != kernel::Font::INVALIDE_GLYPH_INDEX) {
            this->text_length += font->getKernig(prev_index, index);
        }
        this->glyphs.push_back({index, this->text_length});

        this->text_length += font->getAdvance(index);
        prev_index = index;
    }
}

LabelWidget::View::View(LabelWidget* label)
: label{label} {
}

void LabelWidget::View::draw(RenderBatchInterface& batch) {
    auto style = this->label->getStyle();
    assert(style && "GUI::Style is undefined");
    auto font = style->getFont();
    assert(font && "Font is undefined");

    float border = 3;

    glm::vec2 size = this->label->size.get();

    float line_heith = font->getAscender() - font->getDescender();
    float scale      = std::min(size.x / this->label->text_length, size.y / line_heith);

    batch.setColor(style->getColor(Style::Colors::TEXT));

    glm::vec2 pos = glm::vec2(this->label->position.get());
    switch(this->label->alignment.get()) {
        case LabelWidget::Alignment::Center:
            pos.x += 0.5f * (size.x - scale * this->label->text_length);
            break;
        case LabelWidget::Alignment::Right:
            pos.x += size.x - scale * this->label->text_length;
            break;
    }
    pos.y +=
        0.5f * size.y - scale * (0.5 * line_heith + font->getDescender()); // center text verticaly

    for(const auto& it : this->label->glyphs) {
        const auto& glyph = font->getGlypData(it.index);

        glm::vec2 p = pos;
        p.x += scale * it.x;
        batch.drawGlyph(glyph, p, scale);
    }
}

} // namespace bembel::gui
