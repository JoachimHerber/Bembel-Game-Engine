module;
#include "bembel/pch.h"
export module bembel.tools.font_converter:FontView;

import bembel;
import :FontFamily;

namespace bembel::tools {
using namespace bembel::base;
using namespace bembel::kernel;
using namespace bembel::gui;

export class FontView : public Viewport::View {
  public:
    FontView(AssetManager&);
    ~FontView();

    void setFont(FontFamily*);

    virtual void draw(ivec2 const& viewport_position, uvec2 const& viewport_size) override;

  private:
    void update();

    void initFBO();

    void renderToTexture();

    struct Intersection {
        // describes the intersection between a ray and a glyph
        double pos;       // the postilions along the ray
        bool   direction; // indicates whether the ray enters or leaves a glyph
    };
    using Intersections = std::vector<Intersection>;

    void getIntersections(double y, TextureAtlasNode const*, Intersections&);
    void drawGlypeOutline(TextureAtlasNode const*);

    void onResolutionChanged(ivec2 const&);
    void onAtlasUpdate();

  private:
    AssetManager& m_asset_mgr;

    bool m_requires_update = true;

    FontFamily* m_font = nullptr;

    double m_line_width = 4.0f;

    Asset<ShaderProgram> m_shader;

    std::unique_ptr<FrameBufferObject> m_fbo;
    std::unique_ptr<Texture>           m_texture;

    uvec2 m_resolution = {1024, 1024};
};

} // namespace bembel::tools
