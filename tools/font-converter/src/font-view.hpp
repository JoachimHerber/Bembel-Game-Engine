#ifndef BEMBEL_TOOLS_FONTCONVERTER_FONTVIEW_H
#define BEMBEL_TOOLS_FONTCONVERTER_FONTVIEW_H

#include <bembel/bembel.hpp>

#include "font-family.hpp"
#include "glyph-texture-atlas.hpp"
#include "texture-atlas-node.hpp"

class Glyph;

class FontView : public bembel::Viewport::View {
  public:
    FontView(bembel::AssetManager*);
    ~FontView();

    void setFont(FontFamily*);

    virtual void draw(
        const glm::ivec2& viewport_position, const glm::uvec2& viewport_size) override;

  private:
    void update();

    void initFBO();

    void renderToTexture();

    struct Intersection {
        // describes the intersection between a ray and a glyph
        double pos;       // the postilions along the ray
        bool   direction; // indicates whether the ray enters or leaves a glyph
    };
    using IntersectionList = std::vector<Intersection>;

    void getIntersections(double y, const TextureAtlasNode*, IntersectionList&);
    void drawGlypeOutline(const TextureAtlasNode*);

    void onResolutionChanged(const glm::ivec2&);
    void onAtlasUpdate();

  private:
    bembel::AssetManager* asset_mgr;

    bool requires_update = true;

    FontFamily* font = nullptr;

    double line_width = 4.0f;

    bembel::Asset<bembel::ShaderProgram> shader;

    std::unique_ptr<bembel::FrameBufferObject> fbo;
    std::unique_ptr<bembel::Texture>           texture;

    glm::uvec2 resolution;
};

#endif // include guards
