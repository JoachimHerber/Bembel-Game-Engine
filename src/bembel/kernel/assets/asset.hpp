#ifndef BEMBEL_KERNEL_ASSETS_ASSET_HPP
#define BEMBEL_KERNEL_ASSETS_ASSET_HPP

#include "asset-manager.hpp"

namespace bembel::kernel {

template <class AssetType>
class Asset final {
  public:
    Asset();
    Asset(const Asset<AssetType>& other);
    Asset(Asset<AssetType>&& other);
    ~Asset();

    Asset<AssetType>& operator=(const Asset<AssetType>& other);
    Asset<AssetType>& operator=(Asset<AssetType>&& other);

    operator bool() const;

    AssetType* operator->() const;

    void       release();
    AssetType* get() const;

    bool request(AssetManager& asset_mgr, const std::string& file_name);
    bool request(AssetManager& asset_mgr, const base::xml::Element* properties);
    bool get(AssetManager& asset_mgr, const std::string& name);

  private:
    bool set(AssetManager* asset_mgr, AssetHandle handel, bool incrementRefCount);

  private:
    AssetManager* asset_mgr;
    AssetHandle   handel;
};

template <class AssetType>
Asset<AssetType>::Asset()
: asset_mgr(nullptr)
, handel() {
}

template <class AssetType>
Asset<AssetType>::Asset(const Asset<AssetType>& other)
: asset_mgr(other.asset_mgr)
, handel(other.handel) {
    if(this->asset_mgr) { this->asset_mgr->incrementAssetRefCount(this->handel); }
}

template <class AssetType>
Asset<AssetType>::Asset(Asset<AssetType>&& other)
: asset_mgr(other.asset_mgr)
, handel(other.handel) {
    other.asset_mgr = nullptr;
}

template <class AssetType>
Asset<AssetType>::~Asset() {
    if(this->asset_mgr) { this->asset_mgr->releaseAsset(this->handel); }
}

template <class AssetType>
Asset<AssetType>& Asset<AssetType>::operator=(const Asset<AssetType>& other) {
    this->set(other.asset_mgr, other.handel, true);

    return *this;
}

template <class AssetType>
Asset<AssetType>& Asset<AssetType>::operator=(Asset<AssetType>&& other) {
    this->set(other.asset_mgr, other.handel, false);

    other.asset_mgr = nullptr;

    return *this;
}

template <class AssetType>
Asset<AssetType>::operator bool() const {
    return this->asset_mgr && this->asset_mgr->isHandelValid(this->handel);
}

template <class AssetType>
AssetType* Asset<AssetType>::operator->() const {
    return this->asset_mgr->getAsset<AssetType>(this->handel);
}

template <class AssetType>
void Asset<AssetType>::release() {
    if(this->asset_mgr) { this->asset_mgr->releaseAsset(this->handel); }
    this->asset_mgr = nullptr;
}

template <class AssetType>
AssetType* Asset<AssetType>::get() const {
    return this->asset_mgr != nullptr ? this->asset_mgr->getAsset<AssetType>(this->handel)
                                      : nullptr;
}

template <class AssetType>
bool Asset<AssetType>::request(AssetManager& asset_mgr, const std::string& file_name) {
    AssetHandle handl = asset_mgr.requestAsset<AssetType>(file_name);
    return this->set(&asset_mgr, handl, false);
}

template <class AssetType>
bool Asset<AssetType>::request(AssetManager& asset_mgr, const base::xml::Element* properties) {
    AssetHandle handl = asset_mgr.requestAsset<AssetType>(properties);
    return this->set(&asset_mgr, handl, false);
}

template <class AssetType>
bool Asset<AssetType>::get(AssetManager& asset_mgr, const std::string& name) {
    AssetHandle handl = asset_mgr.getAsset<AssetType>(name);
    return this->set(&asset_mgr, handl, true);
}

template <class AssetType>
bool Asset<AssetType>::set(AssetManager* asset_mgr, AssetHandle handel, bool incrementRefCount) {
    if(!asset_mgr->isHandelValid(handel)) return false;

    if(this->asset_mgr) { this->asset_mgr->releaseAsset(this->handel); }

    this->asset_mgr = asset_mgr;
    this->handel    = handel;

    if(incrementRefCount) { this->asset_mgr->incrementAssetRefCount(this->handel); }

    return true;
}

} // namespace bembel::kernel
#endif // include guards
