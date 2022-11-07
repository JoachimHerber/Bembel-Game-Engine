module;
#include "bembel/pch.h"
export module bembel.tools.font_converter:PreviewRenderer;

import bembel;
import :FontFamily;

namespace bembel::tools {
using bembel::base::ivec2;
using bembel::base::uvec2;
using bembel::kernel::TextInputEvent;
using bembel::kernel::Texture;
using bembel::kernel::Viewport;

export class PreviewRenderer : public Viewport::View {
  public:
    PreviewRenderer(FontFamily*, Texture*);
    ~PreviewRenderer();

    virtual void draw(ivec2 const& viewport_position, uvec2 const& viewport_size) override;

    void handleEvent(TextInputEvent& event);

  private:
    void drawGlypes();

  private:
    FontFamily* m_font;
    Texture*    m_texture;

    std::vector<char32_t> m_text;
};

} // namespace bembel::tools
