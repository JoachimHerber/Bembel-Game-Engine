#ifndef BEMBEL_KERNEL_ASSETS_ASSETMANAGER_HPP
#define BEMBEL_KERNEL_ASSETS_ASSETMANAGER_HPP

#include <bembel/base/io/xml.hpp>
#include <bembel/base/logging/logger.hpp>

#include "./asset-container.hpp"
#include "./asset-loader.hpp"
#include "./asset-locator.hpp"

namespace bembel::kernel {

class BEMBEL_API AssetManager final {
  public:
  public:
    AssetManager();
    ~AssetManager();

    /**
     * Adds an asset.
     * The added asset will be managed by the asset manager and its
     * refernence count will be initalised to 1.
     */
    template <typename AssetType>
    AssetHandle addAsset(std::unique_ptr<AssetType> asset, const std::string& name = "");

    /**
     * Request an asset.
     * The requested asset will be loaded if it isn't loaded already.
     * Requesting an asset will increse the reference count of the asset.
     */
    template <typename AssetType>
    AssetHandle requestAsset(const std::string& file_name);
    template <typename AssetType>
    AssetHandle requestAsset(const base::xml::Element* properties);
    AssetHandle requestAsset(const std::string& asset_type_name, const std::string& file_name);
    AssetHandle requestAsset(
        const std::string& asset_type_name, const base::xml::Element* properties);

    /**
     * Releses an asset.
     * This metode will reduce the reference count of the asset and delete
     * the asset once its reference count reaches zero.
     */
    bool releaseAsset(AssetHandle asset_handle);

    void deleteUnusedAssets();

    /**
     * @return true if the asset_handle is valide.
     */
    bool isHandelValid(AssetHandle asset_handle);

    /**
     * @return true if the asset_handle is valide.
     */
    void incrementAssetRefCount(AssetHandle asset_handle);

    void decrementAssetRefCount(AssetHandle asset_handle);

    /**
     * Queries a handle for a named asset.
     * Does not modify the reference count of the asset.
     * @return a handle to the asset with the request name if it exist.
     */
    template <typename AssetType>
    AssetHandle getAssetHandle(const std::string& name);

    /**
     * returns a pointer to the specified asset or a pointer to a dummy asset.
     */
    template <typename AssetType>
    AssetType* getAsset(AssetHandle handle, bool return_dummy_if_handle_invalid = true);

    template <typename AssetType, typename... TArgs>
    bool registerAssetType(TArgs... args);
    template <typename AssetType, typename AssetLoaderType, typename... TArgs>
    bool registerAssetType(TArgs... args);

    AssetLocator& getAssetLocator();

  private:
    template <typename AssetType>
    AssetContainer<AssetType>* getAssetContainer();

  private:
    std::unordered_map<std::string, uint16_t> asset_type_map;

    std::vector<std::shared_ptr<AssetContainerBase>> asset_container;
    std::vector<std::shared_ptr<AssetLoaderBase>>    asset_loader;

    AssetLocator asset_locator;
};

} // namespace bembel::kernel
#include "asset-manager.inl"
#endif // include guards
