module;
#include <string_view>
export module bembel.kernel.assets:Asset;

import bembel.base;

import :AssetHandle;
import :AssetManager;

namespace bembel::kernel {
using namespace bembel::base;

export template <typename T>
concept AssetType = requires(T a) {
    { T::ASSET_TYPE_NAME } -> std::convertible_to<std::string_view>;
};

export template <AssetType T>
class Asset final {
  public:
    Asset() {}
    Asset(Asset<T> const& other) : m_asset_mgr(other.m_asset_mgr), m_handel(other.m_handel) {
        if(m_asset_mgr) { m_asset_mgr->incrementAssetRefCount(m_handel); }
    }
    Asset(Asset<T>&& other) : m_asset_mgr(other.m_asset_mgr), m_handel(other.m_handel) {
        other.m_asset_mgr = nullptr;
    }
    ~Asset() {
        if(m_asset_mgr) { m_asset_mgr->releaseAsset(m_handel); }
    }

    Asset<T>& operator=(Asset<T> const& other) {
        set(other.m_asset_mgr, other.m_handel, true);
        return *this;
    }
    Asset<T>& operator=(Asset<T>&& other) {
        set(other.m_asset_mgr, other.m_handel, false);
        other.m_asset_mgr = nullptr;
        return *this;
    }

    operator bool() const { return m_asset_mgr && m_asset_mgr->isHandelValid(m_handel); }

    T* operator->() const { return m_asset_mgr->getAsset<T>(m_handel); }

    void release() {
        if(m_asset_mgr) { m_asset_mgr->releaseAsset(m_handel); }
        m_asset_mgr = nullptr;
    }

    T* getAsset() const {
        return m_asset_mgr != nullptr ? m_asset_mgr->getAsset<T>(m_handel) : nullptr;
    }

    AssetHandle getHandle() const { return m_handel; }

    bool request(AssetManager& asset_mgr, std::string_view file_name) {
        AssetHandle handl = asset_mgr.requestAsset<T>(file_name);
        return this->set(&asset_mgr, handl, false);
    }

    bool request(AssetManager& asset_mgr, xml::Element const* properties) {
        AssetHandle handl = asset_mgr.requestAsset<T>(properties);
        return this->set(&asset_mgr, handl, false);
    }

    bool get(AssetManager& asset_mgr, std::string_view name) {
        AssetHandle handl = asset_mgr.getAssetHandle<T>(name);
        return set(&asset_mgr, handl, true);
    }

  private:
    bool set(AssetManager* asset_mgr, AssetHandle handel, bool incrementRefCount) {
        if(!asset_mgr->isHandelValid(handel)) return false;

        if(m_asset_mgr) { m_asset_mgr->releaseAsset(m_handel); }

        m_asset_mgr = asset_mgr;
        m_handel    = handel;

        if(incrementRefCount) { m_asset_mgr->incrementAssetRefCount(m_handel); }

        return true;
    }

  private:
    AssetManager* m_asset_mgr = nullptr;
    AssetHandle   m_handel;
};

} // namespace bembel::kernel
