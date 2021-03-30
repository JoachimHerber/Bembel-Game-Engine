#ifndef BEMBEL_GRAPHICS_GEOMETRY_MATERIAL_HPP
#define BEMBEL_GRAPHICS_GEOMETRY_MATERIAL_HPP

#include <bembel/kernel/assets/asset-handle.hpp>
#include <bembel/kernel/assets/asset-loader.hpp>
#include <bembel/kernel/assets/asset-manager.hpp>
#include <bembel/kernel/assets/asset.hpp>
#include <bembel/kernel/open-gl.hpp>
#include <bembel/kernel/rendering/texture.hpp>

namespace bembel::graphics {

class MaterialLoader;
class GraphicSystem;

class BEMBEL_API Material final {
  public:
    using DefaultLoaderType = MaterialLoader;

    using Texture = kernel::Asset<kernel::Texture>;

  public:
    Material(unsigned renderer, GLint);
    Material(const Material&) = delete;
    Material(Material&&)      = delete;
    ~Material();

    unsigned getRenderer() const;
    GLuint   getUniformBufferObject() const;

    const std::vector<Texture>& getTextures() const;
    void                        setTextures(const std::vector<Texture>& textures);
    void                        setTextures(std::vector<Texture>&& textures);

    const static std::string& getTypeName();

  private:
    unsigned renderer = 0;

    GLuint uniform_buffer_object;

    std::vector<Texture> textures;
};

class BEMBEL_API MaterialLoader final : public kernel::AssetLoaderBase {
  public:
    using ContainerType = kernel::AssetContainer<Material>;

    MaterialLoader(kernel::AssetManager&, ContainerType*, GraphicSystem*);
    virtual ~MaterialLoader();

    kernel::AssetHandle requestAsset(const std::string& filename) override;
    kernel::AssetHandle requestAsset(const base::xml::Element* properties) override;
    bool                releaseAsset(kernel::AssetHandle assetHandel) override;

    void deleteUnusedAssets() override;

  private:
    GraphicSystem*        graphic_system;
    kernel::AssetManager& asset_mgr;
    ContainerType*        container;
};

} // namespace bembel::graphics
#endif // include guards
