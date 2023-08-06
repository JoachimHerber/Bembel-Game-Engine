module;
#include <span>
#include <vector>
//
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_IMAGE_H
export module bembel.tools.font_converter:CompositGlyph;

import bembel;

namespace bembel::tools {
using namespace bembel::base;
using namespace bembel::kernel;
using namespace bembel::gui;

export class CompositGlyph {
  public:
    CompositGlyph();
    ~CompositGlyph();

    void init(FT_Face& face, uint, uint);

  private:
  private:

    uint m_id = 0;
};

} // namespace bembel::tools
