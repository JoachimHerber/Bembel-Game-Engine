#ifndef BEMBEL_GRAPHICS_RENDERING_PIPELINE_RENDERINGPIPELINE_HPP
#define BEMBEL_GRAPHICS_RENDERING_PIPELINE_RENDERINGPIPELINE_HPP

#include <bembel/base/io/xml.hpp>
#include <bembel/kernel/assets/asset-manager.hpp>
#include <bembel/kernel/open-gl.hpp>
#include <bembel/kernel/rendering/texture.hpp>
#include <bembel/kernel/scene/scene.hpp>

#include "../texture-view.hpp"

namespace bembel::graphics {

class Camera;
class GeometryRendererBase;
class GraphicSystem;
class RenderingStage;

class BEMBEL_API RenderingPipeline final {
  public:
    RenderingPipeline(GraphicSystem&);
    RenderingPipeline(const RenderingPipeline&) = delete;
    RenderingPipeline& operator=(const RenderingPipeline&) = delete;
    ~RenderingPipeline();

    GraphicSystem&        getGraphicSystem();
    kernel::AssetManager& getAssetManager();

    void              setResulution(const glm::ivec2& value);
    const glm::ivec2& getResulution() const;

    void enable();
    void disable();
    bool isEnabled();

    bool configure(const base::xml::Element*);

    void init();
    void cleanup();

    void                           setScene(std::shared_ptr<kernel::Scene>);
    std::shared_ptr<kernel::Scene> getScene() const;

    std::shared_ptr<Camera> getCamera() const;

    kernel::Texture* getTexture(const std::string& name) const;
    bool             createTexture(const base::xml::Element*);
    bool             createTexture(const std::string& name, GLenum format);

    template <typename StageType>
    StageType* addRenderingStage();

    TextureView*                                     createView(const std::string& texture_name);
    const std::vector<std::unique_ptr<TextureView>>& getViews() const;

    void update();

  private:
    void configureTextures(const base::xml::Element*);
    void configureStages(const base::xml::Element*);
    void configureViews(const base::xml::Element*);
    void configureCamera(const base::xml::Element*);

    GLenum stringToTextureFormat(const std::string&);

  private:
    GraphicSystem& grapic_system;

    glm::ivec2 resolution;
    bool       enabled;
    bool       initalized;

    std::shared_ptr<kernel::Scene> scene;
    std::shared_ptr<Camera>        camera;

    std::map<std::string, std::unique_ptr<kernel::Texture>> textures;
    std::vector<std::unique_ptr<RenderingStage>>            render_stages;

    std::vector<std::unique_ptr<TextureView>> views;
};

template <typename StageType>
inline StageType* RenderingPipeline::addRenderingStage() {
    this->render_stages.push_back(std::make_unique<StageType>(this));
    return static_cast<StageType*>(this->render_stages.back().get());
}

} // namespace bembel::graphics
#endif // include guards
