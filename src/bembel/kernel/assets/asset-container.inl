namespace bembel::kernel {

template <typename AssetType>
inline AssetContainer<AssetType>::AssetContainer(uint16_t type_id)
: asset_type_id(type_id) {
}

template <typename AssetType>
inline AssetContainer<AssetType>::~AssetContainer() {
}

template <typename AssetType>
inline AssetHandle AssetContainer<AssetType>::addAsset(AssetPtr asset) {
    AssetHandle handle;

    // find a place to put the asset
    if(!this->unused_ids.empty()) {
        handle.index   = this->unused_ids.top();
        handle.type_id = this->asset_type_id;

        this->unused_ids.pop();

        this->assets[handle.index].asset           = std::move(asset);
        this->assets[handle.index].reference_count = 0;
        handle.version                             = ++(this->assets[handle.index].version);
    } else {
        handle.index   = uint32_t(this->assets.size());
        handle.type_id = this->asset_type_id;
        handle.version = 1;

        this->assets.push_back({std::move(asset), 1, 0});
    }
    return handle;
}

template <typename AssetType>
inline bool AssetContainer<AssetType>::registerAssetAlias(
    AssetHandle handle, const std::string& alias) {
    if(this->asset_aliasses.find(alias) != this->asset_aliasses.end())
        return false; // alias already in use

    if(!this->isHandelValid(handle)) return false; // invalid handle

    this->asset_aliasses.emplace(alias, handle);
    return true;
}

template <typename AssetType>
inline std::unique_ptr<AssetType> AssetContainer<AssetType>::removeAsset(
    AssetHandle handle, bool force) {
    if(!this->isHandelValid(handle)) return nullptr; // invalid handle

    if(this->assets[handle.index].reference_count > 0) {
        if(force) {
            BEMBEL_LOG_WARNING() << "Removing asset that is still in use." << std::endl;
        } else
            return nullptr;
    }

    AssetPtr asset                             = std::move(this->assets[handle.index].asset);
    this->assets[handle.index].reference_count = -1;
    this->unused_ids.push(handle.index);

    return std::move(asset);
}

template <typename AssetType>
inline AssetHandle AssetContainer<AssetType>::getAssetHandle(const std::string& name) {
    auto it = this->asset_aliasses.find(name);
    if(it == this->asset_aliasses.end()) return AssetHandle();

    if(this->isHandelValid(it->second)) return it->second;

    // The entry in the assetMap refers to an asset, which no longer exist.
    // The entry should hence be removed
    this->asset_aliasses.erase(it);
    return AssetHandle();
}

template <typename AssetType>
inline bool AssetContainer<AssetType>::isHandelValid(AssetHandle handle) {
    if(handle.type_id != this->asset_type_id) return false;
    if(handle.index >= this->assets.size()) return false;
    if(handle.version != this->assets[handle.index].version) return false;
    return this->assets[handle.index].reference_count >= 0;
}

template <typename AssetType>
inline AssetType* AssetContainer<AssetType>::getAsset(
    AssetHandle handle, bool return_dummy_if_handle_invalid) {
    if(this->isHandelValid(handle)) return this->assets[handle.index].asset.get();

    if(return_dummy_if_handle_invalid && this->isHandelValid(this->dummy_asset))
        return this->assets[this->dummy_asset.index].asset.get();

    return nullptr;
}

template <typename AssetType>
inline int AssetContainer<AssetType>::getAssetRefCount(AssetHandle handle) {
    if(!this->isHandelValid(handle)) return -1;

    return this->assets[handle.index].reference_count;
}

template <typename AssetType>
inline void AssetContainer<AssetType>::incrementAssetRefCount(AssetHandle handle) {
    if(this->isHandelValid(handle)) { ++(this->assets[handle.index].reference_count); }
}
template <typename AssetType>
inline void AssetContainer<AssetType>::decrementAssetRefCount(AssetHandle handle) {
    if(this->isHandelValid(handle) && this->assets[handle.index].reference_count > 0) {
        --(this->assets[handle.index].reference_count);
    }
}

template <typename AssetType>
inline void AssetContainer<AssetType>::getAssets(std::vector<AssetHandle>& asset) {
    asset.clear();
    asset.reserve(this->assets.size());

    for(size_t n = 0; n < this->assets.size(); ++n) {
        if(this->assets[n].reference_count >= 0)
            asset.push_back(AssetHandle{uint16_t(n), this->assets[n].version});
    }
}

template <typename AssetType>
inline void AssetContainer<AssetType>::getUnusedAssets(std::vector<AssetHandle>& asset) {
    asset.clear();
    asset.reserve(this->assets.size());

    for(size_t n = 0; n < this->assets.size(); ++n) {
        if(this->assets[n].reference_count == 0)
            asset.push_back(AssetHandle{uint16_t(n), this->assets[n].version});
    }
}

template <typename AssetType>
inline void AssetContainer<AssetType>::setDummyAsset(AssetHandle dummy) {
    this->dummy_asset = dummy;
}

} // namespace bembel::kernel
