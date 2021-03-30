#include "serial-asset-loader.hpp"
namespace bembel::kernel {

template <typename AssetType>
inline SerialAssetLoader<AssetType>::SerialAssetLoader(
    AssetManager& asset_mgr, ContainerType* container)
: asset_mgr(asset_mgr)
, container(container) {
}

template <typename AssetType>
inline SerialAssetLoader<AssetType>::~SerialAssetLoader() {
}

template <typename AssetType>
inline AssetHandle SerialAssetLoader<AssetType>::requestAsset(const std::string& file_name) {
    std::string path;

    auto& locator = this->asset_mgr.getAssetLocator();
    if(!locator.findAssetLocation<AssetType>(file_name, &path)) {
        // can't find the requested file
        return AssetHandle();
    }

    // test if the asset is alredy loaded
    AssetHandle handle = this->container->getAssetHandle(path);
    if(!this->container->isHandelValid(handle)) {
        // we have to load the asset
        std::unique_ptr<AssetType> asset = AssetType::loadAsset(this->asset_mgr, path);
        if(!asset) return AssetHandle();

        handle = this->container->addAsset(std::move(asset));
        this->container->registerAssetAlias(handle, path);
    }

    this->container->incrementAssetRefCount(handle);
    return handle;
}

template <typename AssetType>
inline AssetHandle SerialAssetLoader<AssetType>::requestAsset(
    const base::xml::Element* properties) {
    if(!properties) return AssetHandle();

    std::string name = "";
    if(base::xml::getAttribute(properties, "name", name)) {
        AssetHandle handle = this->container->getAssetHandle(name);
        if(this->container->isHandelValid(handle)) {
            this->container->incrementAssetRefCount(handle);
            return handle;
        }
    }

    std::string file_name = "";
    if(base::xml::getAttribute(properties, "file_name", file_name)) {
        AssetHandle handle = this->requestAsset(file_name);
        if(this->container->isHandelValid(handle) && !name.empty()) {
            this->container->registerAssetAlias(handle, name);
        }
        return handle;
    }

    std::unique_ptr<AssetType> asset = AssetType::createAsset(this->asset_mgr, properties);
    if(!asset) return AssetHandle();

    AssetHandle handle = this->container->addAsset(std::move(asset));
    this->container->incrementAssetRefCount(handle);
    if(!name.empty()) this->container->registerAssetAlias(handle, name);

    return handle;
}

template <typename AssetType>
inline bool SerialAssetLoader<AssetType>::releaseAsset(AssetHandle asset_handel) {
    if(this->container->isHandelValid(asset_handel)) {
        this->container->decrementAssetRefCount(asset_handel);
        if(this->container->getAssetRefCount(asset_handel) == 0) {
            AssetType::deleteAsset(this->asset_mgr, this->container->removeAsset(asset_handel));
            return true;
        }
    }
    return false;
}

template <typename AssetType>
inline void SerialAssetLoader<AssetType>::deleteUnusedAssets() {
    std::vector<AssetHandle> unusedAssets;
    this->container->getUnusedAssets(unusedAssets);
    for(auto hndl : unusedAssets) {
        AssetType::deleteAsset(this->asset_mgr, this->container->removeAsset(hndl));
    }
}

} // namespace bembel::kernel
