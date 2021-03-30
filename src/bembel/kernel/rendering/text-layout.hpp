#ifndef BEMBEL_KERNEL_RENDERING_TEXT_LAYOUT_HPP
#define BEMBEL_KERNEL_RENDERING_TEXT_LAYOUT_HPP

#include <bembel/kernel/assets/asset-handle.hpp>
#include <bembel/kernel/assets/asset.hpp>
#include <bembel/kernel/assets/serial-asset-loader.hpp>
#include <bembel/library.hpp>

#include "font.hpp"

namespace bembel::kernel {

class BEMBEL_API UTF8TextIterator {
  public:
    UTF8TextIterator(std::string_view text, std::size_t index = 0);
    UTF8TextIterator(UTF8TextIterator&&)      = default;
    UTF8TextIterator(const UTF8TextIterator&) = default;
    ~UTF8TextIterator()                       = default;

    UTF8TextIterator begin();
    UTF8TextIterator end();

    char32_t          operator*();
    bool              operator!=(const UTF8TextIterator& other);
    UTF8TextIterator& operator++();

  private:
    unsigned getNumBytes() const;

  private:
    std::string_view text;
    std::size_t      index;
};

class BEMBEL_API TextLayout final {
  public:
    TextLayout()  = default;
    ~TextLayout() = default;

    void        setFont(Font* font);
    const Font* getFont() const;

    void setText(std::string_view text);

    enum class VerticalAlignment { Top, Center, Buttom, Baseline };
    enum class HorizontalAlignment { Left, Center, Right };

    void calculateSimpleLayout(
        VerticalAlignment   v_align = VerticalAlignment::Baseline,
        HorizontalAlignment h_align = HorizontalAlignment::Center,
        bool                bold    = false,
        bool                oblique = false);

    struct Glyph {
        glm::vec2 pos;
        unsigned  id;
    };
    const std::vector<Glyph>& getGlyphs() const { return this->glyphs; }

  private:
    Font*              font = nullptr;
    std::string_view   text;
    std::vector<Glyph> glyphs;

    struct Line {
        std::size_t first_glyph_index;
        std::size_t num_glyphs;
        float       lenght;
    };
};

} // namespace bembel::kernel

#endif // include guards
