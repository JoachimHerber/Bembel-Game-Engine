#ifndef BEMBEL_KERNEL_ASSETS_ASSETCONTAINER_HPP
#define BEMBEL_KERNEL_ASSETS_ASSETCONTAINER_HPP

#include <bembel/base/logging/logger.hpp>

#include "./asset-handle.hpp"

namespace bembel::kernel {

class AssetContainerBase {
  public:
    AssetContainerBase()          = default;
    virtual ~AssetContainerBase() = default;

    virtual bool isHandelValid(AssetHandle handle) = 0;

    virtual int  getAssetRefCount(AssetHandle handle) = 0;
    virtual void incrementAssetRefCount(AssetHandle)  = 0;
    virtual void decrementAssetRefCount(AssetHandle)  = 0;

    virtual void getUnusedAssets(std::vector<AssetHandle>& asset) = 0;
};

template <typename AssetType>
class AssetContainer : public AssetContainerBase {
  public:
    using AssetPtr = std::unique_ptr<AssetType>;

  public:
    AssetContainer(uint16_t typeId);
    ~AssetContainer();

    /**
     * Adds an asset container to the container.
     * @param[in] asset
     *  The asset that should be added to the container.
     * @return
     *  A handle that can be used to retrieve the asset.
     */
    AssetHandle addAsset(AssetPtr asset);

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
    bool registerAssetAlias(AssetHandle handle, const std::string& alias);

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
    AssetPtr removeAsset(AssetHandle handle, bool force = false);

    /**
     * Returns the a handle for the requested asset.
     * @param[in] name
     *  The name of the requested asset.
     *  The 'name' must have been register as an alias for the asset (see
     *RegisterAssetAlias).
     */
    AssetHandle getAssetHandle(const std::string& name);

    /**
     * return true if the specified handle points to a valid asset.
     */
    bool isHandelValid(AssetHandle handle) override;

    /**
     * returns true if the specified name has been register as an alias for an
     * asset.
     */
    bool hasAsset(const std::string& name) { return isHandelValid(getAssetHandle(name)); }

    /**
     * Returns the asset specified by the handle or a dummy asset if the specified
     * handle is infalid;
     */
    AssetType* getAsset(AssetHandle handle, bool return_dummy_if_handle_invalid = true);

    /**
     * Return the reference count for the asset specified by the handle.
     */
    int getAssetRefCount(AssetHandle handle) override;

    /**
     * Increments the reference count of the asset specified by the handle.
     */
    void incrementAssetRefCount(AssetHandle handle) override;

    /**
     * Decrements the reference count of the asset specified by the handle.
     */
    void decrementAssetRefCount(AssetHandle handle) override;

    /**
     * Fills the specified vector with handles to all assets contained in this
     * container.
     */
    void getAssets(std::vector<AssetHandle>& assets);

    /**
     * Fills the specified vector with handles to all assets contained
     * in this container for which the reference count is zero.
     */
    void getUnusedAssets(std::vector<AssetHandle>& asset) override;

    /**
     * Specifies the asset that is returned by GetAsset() when it
     * is called with an invalid asset-handle.
     */
    void setDummyAsset(AssetHandle dummy);

  protected:
    struct AssetData {
        AssetPtr asset;
        uint16_t version;
        int32_t  reference_count;
    };

    uint16_t asset_type_id;

    std::map<std::string, AssetHandle> asset_aliasses;

    std::vector<AssetData> assets;
    std::stack<unsigned>   unused_ids;

    AssetHandle dummy_asset;
};

} // namespace bembel::kernel
#include "asset-container.inl"
#endif // include guards
