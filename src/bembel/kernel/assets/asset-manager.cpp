#include "./asset-manager.hpp"

#include <bembel/base/logging/logger.hpp>

namespace bembel::kernel {

AssetManager::AssetManager() {
}

AssetManager::~AssetManager() {
}

AssetHandle AssetManager::requestAsset(
    const std::string& asset_type_name, const std::string& filename) {
    auto it = this->asset_type_map.find(asset_type_name);
    if(it == this->asset_type_map.end()) return AssetHandle();

    return this->asset_loader[it->second]->requestAsset(filename);
}

AssetHandle AssetManager::requestAsset(
    const std::string& asset_type_name, const base::xml::Element* properties) {
    auto it = this->asset_type_map.find(asset_type_name);
    if(it == this->asset_type_map.end()) return AssetHandle();

    return this->asset_loader[it->second]->requestAsset(properties);
}

bool AssetManager::releaseAsset(AssetHandle asset_handel) {
    if(asset_handel.type_id >= this->asset_loader.size()) return false;

    return this->asset_loader[asset_handel.type_id]->releaseAsset(asset_handel);
}

void AssetManager::deleteUnusedAssets() {
    for(auto loder : this->asset_loader) loder->deleteUnusedAssets();
}

bool AssetManager::isHandelValid(AssetHandle asset_handel) {
    if(asset_handel.type_id >= this->asset_container.size()) return false;

    return this->asset_container[asset_handel.type_id]->isHandelValid(asset_handel);
}

void AssetManager::incrementAssetRefCount(AssetHandle asset_handel) {
    if(asset_handel.type_id >= this->asset_container.size()) return;

    this->asset_container[asset_handel.type_id]->incrementAssetRefCount(asset_handel);
}
void AssetManager::decrementAssetRefCount(AssetHandle asset_handel) {
    if(asset_handel.type_id >= this->asset_container.size()) return;

    this->asset_container[asset_handel.type_id]->decrementAssetRefCount(asset_handel);
}

AssetLocator& AssetManager::getAssetLocator() {
    return this->asset_locator;
}

} // end of namespace bembel::kernel
