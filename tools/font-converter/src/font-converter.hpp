#ifndef BEMBEL_TOOLS_FONTCONVERTER_FONT_CONVERTER_H
#define BEMBEL_TOOLS_FONTCONVERTER_FONT_CONVERTER_H

#include <bembel/bembel.hpp>

#include "./font-family.hpp"

class FontConverter {
  public:
    FontConverter();
    ~FontConverter();

    bool loade(const std::vector<std::string>&);

  private:
    bool loadeFile(const std::string&);

  private:
    FT_Library library;

    std::vector<char32_t> characters;

    std::map<std::string, std::unique_ptr<FontFamily>> fonts;
};

#endif // include guards
