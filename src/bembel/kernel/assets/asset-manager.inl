namespace bembel::kernel {

template <typename AssetType>
inline AssetContainer<AssetType>* AssetManager::getAssetContainer() {
    auto it = this->asset_type_map.find(AssetType::getTypeName());
    if(it == this->asset_type_map.end()) return nullptr;

    return static_cast<AssetContainer<AssetType>*>(this->asset_container[it->second].get());
}

template <typename AssetType>
inline AssetHandle AssetManager::addAsset(
    std::unique_ptr<AssetType> asset, const std::string& name) {
    auto container = this->getAssetContainer<AssetType>();
    if(container) {
        auto handle = container->addAsset(std::move(asset));
        container->incrementAssetRefCount(handle);
        if(!name.empty()) container->registerAssetAlias(handle, name);
        return handle;
    }
    return AssetHandle();
}

template <typename AssetType>
inline AssetHandle AssetManager::requestAsset(const std::string& file_name) {
    return this->requestAsset(AssetType::getTypeName(), file_name);
}
template <typename AssetType>
inline AssetHandle AssetManager::requestAsset(const xml::Element* properties) {
    return this->requestAsset(AssetType::getTypeName(), properties);
}

template <typename AssetType>
inline AssetHandle AssetManager::getAssetHandle(const std::string& name) {
    auto container = this->getAssetContainer<AssetType>();
    if(container) return container->getAssetHandle(name);
    return AssetHandle();
}

template <typename AssetType>
inline AssetType* AssetManager::getAsset(AssetHandle handle, bool return_dummy_if_handle_invalid) {
    auto container = this->getAssetContainer<AssetType>();
    if(container) return container->getAsset(handle, return_dummy_if_handle_invalid);
    return nullptr;
}

template <typename AssetType, typename... TArgs>
inline bool AssetManager::registerAssetType(TArgs... args) {
    auto it = this->asset_type_map.find(AssetType::getTypeName());
    if(it != this->asset_type_map.end()) return false; // Asset type has already been registered

    assert(this->asset_type_map.size() < 0xFFFFLU && "number of asset-types surpasses the maximum");
    uint16_t type_id = uint16_t(this->asset_type_map.size());

    auto container = std::make_shared<AssetContainer<AssetType>>(type_id);
    auto loader =
        std::make_shared<typename AssetType::DefaultLoaderType>(*this, container.get(), args...);

    this->asset_type_map.emplace(AssetType::getTypeName(), type_id);
    this->asset_container.push_back(container);
    this->asset_loader.push_back(loader);
    return true;
}

template <typename AssetType, typename AssetLoaderType, typename... TArgs>
inline bool AssetManager::registerAssetType(TArgs... args) {
    auto it = this->asset_type_map.find(AssetType::getTypeName());
    if(it != this->asset_type_map.end()) return false; // Asset type has already been registered

    assert(this->asset_type_map.size() < 0xFFFFLU && "number of asset-types surpasses the maximum");
    uint16_t type_id = this->asset_type_map.size();

    auto container = std::make_shared<AssetContainer<AssetType>>(type_id);
    auto loader    = std::make_shared<AssetLoaderType>(*this, container.get(), args...);

    this->asset_type_map.emplace(AssetType::getTypeName(), type_id);
    this->asset_container.push_back(container);
    this->asset_loader.push_back(loader);
    return true;
}

} // namespace bembel::kernel
