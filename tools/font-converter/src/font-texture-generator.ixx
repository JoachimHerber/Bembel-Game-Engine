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
    FontTextureGenerator();

    void setResolution(uint);

    void generateTexture(FontFamily& font, GlyphTextureAtlas& texture_atlas, uint max_dist);

    bool saveTexture(std::filesystem::path path);

    FrameBufferObject* getFBO()        { return m_fbo.get();     }
    Texture*           getTexture()    { return m_texture.get(); }
    uint               getResolution() { return m_resolution;    }

  private:
    void renderToTexture(FontFamily& font, GlyphTextureAtlas& texture_atlas, uint max_dist);

    struct Intersection {
        // describes the intersection between a ray and a glyph
        double pos;       // the postilions along the ray
        bool   direction; // indicates whether the ray enters or leaves a glyph

        bool operator!=(Intersection other) { return pos != other.pos || direction != other.direction; }
    };
    using Intersections = std::vector<Intersection>;

    void getIntersections(int row, TextureAtlasNode const*, Intersections&);
    void drawGlypeOutline(TextureAtlasNode const*, double margin);

  private:
    Asset<ShaderProgram> m_shader;

    std::unique_ptr<FrameBufferObject> m_fbo;
    std::unique_ptr<Texture>           m_texture;

    uint m_resolution = 1024;

};

} // namespace bembel::tools