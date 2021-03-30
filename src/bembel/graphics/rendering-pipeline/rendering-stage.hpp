#ifndef BEMBEL_GRAPHICS_RENDERING_PIPELINE_RENDERINGSTAGE_HPP
#define BEMBEL_GRAPHICS_RENDERING_PIPELINE_RENDERINGSTAGE_HPP

#include <bembel/kernel/assets/asset-manager.hpp>
#include <bembel/kernel/rendering/frame-buffer-object.hpp>
#include <bembel/kernel/rendering/texture.hpp>
#include <bembel/kernel/scene/scene.hpp>

namespace bembel::graphics {

class RenderingPipeline;

class BEMBEL_API RenderingStage {
  public:
    RenderingStage(RenderingPipeline& pipline);
    virtual ~RenderingStage();

    virtual void setScene(kernel::Scene*);

    virtual void init();
    virtual void cleanup();

    virtual void execute() = 0;

  protected:
    kernel::AssetManager& getAssetManager();

    void setInputTextures(const std::vector<std::string>&);

    void setDepthOutputTexture(const std::string&);
    void setColorOutputTexture(unsigned index, const std::string&);

    void bindInputTextures();
    void releaseInputTextures();

  protected:
    RenderingPipeline& pipline;

    std::vector<kernel::Texture*>              textures;
    std::unique_ptr<kernel::FrameBufferObject> fbo;
};

} // namespace bembel::graphics
#endif // include guards
