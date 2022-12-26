module;
#include <memory>
#include <string_view>
#include <utility>
module bembel.graphics;

import bembel.base;
import bembel.kernel;
import bembel.graphics.geometry;
import bembel.graphics.pipeline;

namespace bembel::graphics {
using namespace bembel::base;
using namespace bembel::kernel;

class MaterialLoader final : public AssetLoaderBase {
  public:
    using ContainerType = AssetContainer<Material>;

    MaterialLoader(ContainerType* container, GraphicSystem* graphic_system)
      : m_graphic_system(graphic_system), m_container(container) {}
    virtual ~MaterialLoader() = default;

    AssetHandle requestAsset(std::string_view filename) override {
        kernel::AssetHandle handle = m_container->getAssetHandle(filename);

        if(!m_container->isHandelValid(handle)) {
            // we have to load the asset
            std::unique_ptr<Material> asset = nullptr;
            // Material::LoadAsset( _assetMgr, fileName );
            if(!asset) return kernel::AssetHandle();

            handle = m_container->addAsset(std::move(asset));
            m_container->incrementAssetRefCount(handle);
            m_container->registerAssetAlias(handle, filename);
        }

        m_container->incrementAssetRefCount(handle);
        return handle;
    }

    AssetHandle requestAsset(xml::Element const* properties) override {
        std::string name = "";
        if(!xml::getAttribute(properties, "name", name)) return AssetHandle();

        AssetHandle handle = m_container->getAssetHandle(name);
        if(!m_container->isHandelValid(handle)) {
            std::string renderer_name;
            xml::getAttribute(properties, "renderer", renderer_name);
            auto renderer = m_graphic_system->getRenderer(renderer_name);
            if(!renderer) return AssetHandle();

            // we have to load the asset
            std::unique_ptr<Material> asset = renderer->createMaterial(properties);
            if(!asset) return AssetHandle();

            handle = m_container->addAsset(std::move(asset));
            m_container->registerAssetAlias(handle, name);
        }

        m_container->incrementAssetRefCount(handle);
        return handle;
    }
    bool releaseAsset(AssetHandle asset_handel) override {
        if(m_container->isHandelValid(asset_handel)) {
            m_container->decrementAssetRefCount(asset_handel);
            if(m_container->getAssetRefCount(asset_handel) == 0) {
                auto mat = m_container->removeAsset(asset_handel);

                return true;
            }
        }
        return false;
    }

    void deleteUnusedAssets() override {
        std::vector<AssetHandle> unusedAssets;
        m_container->getUnusedAssets(unusedAssets);
        for(auto hndl : unusedAssets) { auto mat = m_container->removeAsset(hndl); }
    }

  private:
    GraphicSystem* m_graphic_system;
    ContainerType* m_container;
};

GraphicSystem::GraphicSystem(Engine& engine) : System("Graphics"), m_engine{engine} {
    assets::registerAssetType<Material, MaterialLoader>(this);
    assets::registerAssetType<GeometryMesh>();
    assets::registerAssetType<GeometryModel>();
    assets::registerAssetType<Shader>();
    assets::registerAssetType<ShaderProgram>();

    using Stage = RenderingPipeline::Stage;
    Stage::registerStageType<GeometryRenderingStage>("DeferredGeometryStage");
    Stage::registerStageType<DeferredLightingStage>("DeferredLightingStage");
    Stage::registerStageType<EnvironmentMapReflectionStage>("EnvironmentMapReflectionStage");
}
GraphicSystem::~GraphicSystem() {
    events::removeHandler<WindowUpdateEvent>(this);
    events::removeHandler<FrameBufferResizeEvent>(this);
}

RenderingPipeline* GraphicSystem::createRenderingPipline() {
    m_pipelines.push_back(std::make_unique<RenderingPipeline>(m_engine.display));
    return m_pipelines.back().get();
}

std::vector<std::shared_ptr<RenderingPipeline>> const& GraphicSystem::getRenderingPipelines() {
    return m_pipelines;
}

std::vector<std::shared_ptr<GeometryRendererBase>> const& GraphicSystem::getRenderer() const {
    return m_renderer;
}

GeometryRendererBase* GraphicSystem::getRenderer(std::string_view name) const {
    auto it = m_renderer_map.find(name);
    if(it != m_renderer_map.end())
        return m_renderer[it->second].get();
    else
        return nullptr;
}

bool GraphicSystem::configure(xml::Element const* properties) {
    if(!properties) return false;

    configureRenderer(properties->FirstChildElement("Renderer"));
    configurePipelines(properties->FirstChildElement("RenderingPipelines"));
    events::broadcast<InitGraphicResourcesEvent>();
    return true;
}

bool GraphicSystem::init() {
    for(auto& pipline : m_pipelines) pipline->init();

    events::broadcast<InitGraphicResourcesEvent>();
    return true;
}

void GraphicSystem::shutdown() {
    m_pipelines.clear();
    events::broadcast<CleanuptGraphicResourcesEvent>();
}

void GraphicSystem::update(double) {
    for(auto& pipline : m_pipelines) pipline->update(m_geometry_render_queue, m_renderer);
}

void GraphicSystem::configureRenderer(xml::Element const* properties) {
    if(!properties) return;

    for(auto renderer_properties : xml::IterateChildElements(properties)) {
        RendererPtr renderer =
            DefaultGeometryRenderer::createRenderer(renderer_properties, uint(m_renderer.size()));

        if(renderer) {
            std::string name;
            if(xml::getAttribute(renderer_properties, "name", name))
                m_renderer_map.emplace(name, m_renderer.size());

            m_renderer.push_back(std::move(renderer));
        }
    }
}
void GraphicSystem::configurePipelines(xml::Element const* properties) {
    if(!properties) return;

    for(auto pipeline_properties : xml::IterateChildElements(properties, "RenderingPipeline")) {
        auto pipline = std::make_unique<RenderingPipeline>(m_engine.display);
        if(!pipline->configure(pipeline_properties)) pipline.reset();

        m_pipelines.push_back(std::move(pipline));
    }
}

} // namespace bembel::graphics
