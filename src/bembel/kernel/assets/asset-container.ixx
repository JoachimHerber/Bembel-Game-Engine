module;
#include "bembel/pch.h"
export module bembel.kernel.assets:AssetContainer;

import bembel.base;

import :AssetHandle;

namespace bembel::kernel {
using namespace bembel::base;

export class AssetContainerBase {
  public:
    AssetContainerBase()          = default;
    virtual ~AssetContainerBase() = default;

    virtual bool isHandelValid(AssetHandle handle) = 0;

    virtual int  getAssetRefCount(AssetHandle handle) = 0;
    virtual void incrementAssetRefCount(AssetHandle)  = 0;
    virtual void decrementAssetRefCount(AssetHandle)  = 0;

    virtual void getUnusedAssets(std::vector<AssetHandle>& asset) = 0;
};

export template <typename AssetType>
class AssetContainer : public AssetContainerBase {
  public:
    using AssetPtr = std::unique_ptr<AssetType>;

  public:
    AssetContainer(u16 type_id) : m_asset_type_id{type_id} {}
    ~AssetContainer() = default;

    /**
     * Adds an asset container to the container.
     * @param[in] asset
     *  The asset that should be added to the container.
     * @return
     *  A handle that can be used to retrieve the asset.
     */
    AssetHandle addAsset(AssetPtr asset) {
        AssetHandle handle;

        // find a place to put the asset
        if(!m_unused_ids.empty()) {
            handle.index   = m_unused_ids.top();
            handle.type_id = m_asset_type_id;

            m_unused_ids.pop();

            m_assets[handle.index].asset           = std::move(asset);
            m_assets[handle.index].reference_count = 0;
            handle.version                         = ++(m_assets[handle.index].version);
        } else {
            handle.index   = uint32_t(m_assets.size());
            handle.type_id = m_asset_type_id;
            handle.version = 1;

            m_assets.push_back({std::move(asset), 1, 0});
        }
        return handle;
    }

    /**
     * Registers an alias for an asset.
     * The alias can than be used to to restive the asset handle.
     * @param[in] asset
     *  The handle that specifies the asset.
     * @param[in] handle
     *  The alias that should be used for the asset.
     * @return
     *  true if successful.
     *  false it handle is invalid or alias is already in use.
     */
    bool registerAssetAlias(AssetHandle handle, std::string_view alias) {
        if(m_asset_aliasses.find(alias) != m_asset_aliasses.end())
            return false; // alias already in use

        if(!isHandelValid(handle)) return false; // invalid handle

        m_asset_aliasses.emplace(alias, handle);
        return true;
    }

    /**
     * removes an asset from the container
     * @param[in] handle
     *  The handle to the asset that should be removed.
     * @param[in] force
     *  If true, the asset will be removed even if its reference count is
     *non-zero. If false, the asset won't be removed if its reference count is
     *non-zero.
     * @return
     *  True if an asset has been removed.
     */
    AssetPtr removeAsset(AssetHandle handle, bool force = false) {
        if(!isHandelValid(handle)) return nullptr; // invalid handle

        if(m_assets[handle.index].reference_count > 0) {
            if(force) {
                log().warning("Removing asset that is still in use.");
            } else
                return nullptr;
        }

        AssetPtr asset                         = std::move(m_assets[handle.index].asset);
        m_assets[handle.index].reference_count = -1;
        m_unused_ids.push(handle.index);

        return std::move(asset);
    }

    /**
     * Returns the a handle for the requested asset.
     * @param[in] name
     *  The name of the requested asset.
     *  The 'name' must have been register as an alias for the asset (see
     *RegisterAssetAlias).
     */
    AssetHandle getAssetHandle(std::string_view name) {
        auto it = m_asset_aliasses.find(name);
        if(it == m_asset_aliasses.end()) return AssetHandle();

        if(isHandelValid(it->second)) return it->second;

        // The entry in the assetMap refers to an asset, which no longer exist.
        // The entry should hence be removed
        m_asset_aliasses.erase(it);
        return AssetHandle();
    }

    /**
     * return true if the specified handle points to a valid asset.
     */
    bool isHandelValid(AssetHandle handle) override {
        if(handle.type_id != m_asset_type_id) return false;
        if(handle.index >= m_assets.size()) return false;
        if(handle.version != m_assets[handle.index].version) return false;
        return m_assets[handle.index].reference_count >= 0;
    }

    /**
     * returns true if the specified name has been register as an alias for an
     * asset.
     */
    bool hasAsset(std::string_view name) { return isHandelValid(getAssetHandle(name)); }

    /**
     * Returns the asset specified by the handle or a dummy asset if the specified
     * handle is infalid;
     */
    AssetType* getAsset(AssetHandle handle, bool return_dummy_if_handle_invalid = true) {
        if(isHandelValid(handle)) return m_assets[handle.index].asset.get();

        if(return_dummy_if_handle_invalid && isHandelValid(m_dummy_asset))
            return m_assets[m_dummy_asset.index].asset.get();

        return nullptr;
    }

    /**
     * Return the reference count for the asset specified by the handle.
     */
    int getAssetRefCount(AssetHandle handle) override {
        if(!isHandelValid(handle)) return -1;

        return m_assets[handle.index].reference_count;
    }

    /**
     * Increments the reference count of the asset specified by the handle.
     */
    void incrementAssetRefCount(AssetHandle handle) override {
        if(isHandelValid(handle)) { ++(m_assets[handle.index].reference_count); }
    }

    /**
     * Decrements the reference count of the asset specified by the handle.
     */
    void decrementAssetRefCount(AssetHandle handle) override {
        if(isHandelValid(handle) && m_assets[handle.index].reference_count > 0) {
            --(m_assets[handle.index].reference_count);
        }
    }

    /**
     * Fills the specified vector with handles to all assets contained in this
     * container.
     */
    void getAssets(std::vector<AssetHandle>& assets) {
        assets.clear();
        assets.reserve(m_assets.size());

        for(size_t n = 0; n < m_assets.size(); ++n) {
            if(m_assets[n].reference_count >= 0)
                assets.push_back(AssetHandle{uint16_t(n), m_assets[n].version});
        }
    }

    /**
     * Fills the specified vector with handles to all assets contained
     * in this container for which the reference count is zero.
     */
    void getUnusedAssets(std::vector<AssetHandle>& assets) override {
        assets.clear();
        assets.reserve(m_assets.size());

        for(size_t n = 0; n < m_assets.size(); ++n) {
            if(m_assets[n].reference_count == 0)
                assets.push_back(AssetHandle{uint16_t(n), m_assets[n].version});
        }
    }

    /**
     * Specifies the asset that is returned by GetAsset() when it
     * is called with an invalid asset-handle.
     */
    void setDummyAsset(AssetHandle dummy) { m_dummy_asset = dummy; }

  protected:
    struct AssetData {
        AssetPtr  asset;
        base::u16 version;
        base::i32 reference_count;
    };

    base::u16 m_asset_type_id;

    Dictionary<AssetHandle> m_asset_aliasses;

    std::vector<AssetData> m_assets;
    std::stack<unsigned>   m_unused_ids;

    AssetHandle m_dummy_asset;
};

} // namespace bembel::kernel
