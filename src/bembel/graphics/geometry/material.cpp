#include "./material.hpp"

#include <bembel/base/utils/conversion.hpp>

#include "../graphic-system.hpp"
#include "./renderer.hpp"

namespace bembel::graphics {

Material::Material(unsigned renderer, GLint size)
: renderer(renderer) {
    glGenBuffers(1, &(this->uniform_buffer_object));
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, this->uniform_buffer_object);
    glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_STATIC_DRAW);
}

Material::~Material() {
    glDeleteBuffers(1, &(this->uniform_buffer_object));
}

unsigned Material::getRenderer() const {
    return this->renderer;
}

GLuint Material::getUniformBufferObject() const {
    return this->uniform_buffer_object;
}

const std::vector<Material::Texture>& Material::getTextures() const {
    return this->textures;
}

void Material::setTextures(const std::vector<Texture>& textures) {
    this->textures = textures;
}
void Material::setTextures(std::vector<Texture>&& textures) {
    this->textures = textures;
}

const std::string& Material::getTypeName() {
    const static std::string type_name = "Material";
    return type_name;
}

MaterialLoader::MaterialLoader(
    kernel::AssetManager& asset_mgr, ContainerType* container, GraphicSystem* graphic_system)
: graphic_system(graphic_system)
, asset_mgr(asset_mgr)
, container(container) {
}

MaterialLoader::~MaterialLoader() {
}

kernel::AssetHandle MaterialLoader::requestAsset(const std::string& fileName) {
    kernel::AssetHandle handle = this->container->getAssetHandle(fileName);

    if(!this->container->isHandelValid(handle)) {
        // we have to load the asset
        std::unique_ptr<Material> asset = nullptr;
        // Material::LoadAsset( _assetMgr, fileName );
        if(!asset) return kernel::AssetHandle();

        handle = this->container->addAsset(std::move(asset));
        this->container->incrementAssetRefCount(handle);
        this->container->registerAssetAlias(handle, fileName);
    }

    this->container->incrementAssetRefCount(handle);
    return handle;
}

kernel::AssetHandle MaterialLoader::requestAsset(const base::xml::Element* properties) {
    std::string name = "";
    if(!base::xml::getAttribute(properties, "name", name)) return kernel::AssetHandle();

    kernel::AssetHandle handle = this->container->getAssetHandle(name);
    if(!this->container->isHandelValid(handle)) {
        std::string renderer_name;
        base::xml::getAttribute(properties, "renderer", renderer_name);
        auto renderer = this->graphic_system->getRenderer(renderer_name);
        if(!renderer) return kernel::AssetHandle();

        // we have to load the asset
        std::unique_ptr<Material> asset = renderer->createMaterial(properties);
        if(!asset) return kernel::AssetHandle();

        handle = this->container->addAsset(std::move(asset));
        this->container->registerAssetAlias(handle, name);
    }

    this->container->incrementAssetRefCount(handle);
    return handle;
}

bool MaterialLoader::releaseAsset(kernel::AssetHandle asset_handel) {
    if(this->container->isHandelValid(asset_handel)) {
        this->container->decrementAssetRefCount(asset_handel);
        if(this->container->getAssetRefCount(asset_handel) == 0) {
            auto mat = this->container->removeAsset(asset_handel);

            return true;
        }
    }
    return false;
}

void MaterialLoader::deleteUnusedAssets() {
    std::vector<kernel::AssetHandle> unusedAssets;
    this->container->getUnusedAssets(unusedAssets);
    for(auto hndl : unusedAssets) { auto mat = this->container->removeAsset(hndl); }
}

} // namespace bembel::graphics
