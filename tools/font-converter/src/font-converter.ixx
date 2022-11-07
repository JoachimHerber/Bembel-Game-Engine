module;
#include "bembel/pch.h"
//
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_IMAGE_H
export module bembel.tools.font_converter:FontConverter;

import :FontFamily;

namespace bembel::tools {
using bembel::base::Dictionary;
using bembel::base::In;
using bembel::base::Signal;

export class FontConverter {
  public:
    FontConverter();
    ~FontConverter();

    bool loade(In<std::filesystem::path>);

    FontFamily* getFontFamily(int index) {
        if(0 <= index && index < m_fonts.size()) return m_fonts[index].get();
        return nullptr;
    }

    bool parseGlypes();

    Signal<std::string_view> font_family_added_signal;

  private:
    bool loadeFile(In<std::filesystem::path>);

  private:
    FT_Library              m_library;
    Dictionary<FontFamily*> m_font_dictionary;

    std::vector<std::unique_ptr<FontFamily>> m_fonts;
};

} // namespace bembel::tools
