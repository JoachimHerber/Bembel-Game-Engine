module;
#include <filesystem>
export module bembel.tools.font_converter:FontTextureGenerator;

import bembel;
import :FontFamily;
import :GlyphTextureAtlas;

namespace bembel::tools {
using namespace bembel::base;
using namespace bembel::kernel;
using namespace bembel::gui;

export class FontTextureGenerator {
  public:
    FontTextureGenerator(AssetManager&);

    void setResolution(uvec2);

    void generateTexture(FontFamily& font, GlyphTextureAtlas& texture_atlas, double line_width);

    bool saveTexture(std::filesystem::path path);

    FrameBufferObject* getFBO() { return m_fbo.get(); }
    Texture*           getTexture() { return m_texture.get(); }
    uvec2              getResolution() { return m_resolution; }

  private:
    void renderToTexture(FontFamily& font, GlyphTextureAtlas& texture_atlas, double line_width);

    struct Intersection {
        // describes the intersection between a ray and a glyph
        double pos;       // the postilions along the ray
        bool   direction; // indicates whether the ray enters or leaves a glyph
    };
    using Intersections = std::vector<Intersection>;

    void getIntersections(double y, TextureAtlasNode const*, Intersections&);
    void drawGlypeOutline(TextureAtlasNode const*, double line_width);

  private:
    AssetManager& m_asset_mgr;

    Asset<ShaderProgram> m_shader;

    std::unique_ptr<FrameBufferObject> m_fbo;
    std::unique_ptr<Texture>           m_texture;

    uvec2 m_resolution = {1024, 1024};

};

} // namespace bembel::tools