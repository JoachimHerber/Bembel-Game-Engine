#ifndef BEMBEL_KERNEL_ASSETS_SERIALASSETLOADER_HPP
#define BEMBEL_KERNEL_ASSETS_SERIALASSETLOADER_HPP

#include <bembel/library.hpp>

#include "./asset-loader.hpp"
#include "./asset-locator.hpp"
#include "./asset-manager.hpp"

namespace bembel::kernel {

template <typename AssetType>
class SerialAssetLoader : public AssetLoaderBase {
  public:
    using ContainerType = AssetContainer<AssetType>;

    SerialAssetLoader(AssetManager& asset_mgr, ContainerType* container);
    virtual ~SerialAssetLoader();

    AssetHandle requestAsset(const std::string& file_name) override;
    AssetHandle requestAsset(const base::xml::Element* properties) override;
    bool        releaseAsset(AssetHandle asset_handel) override;
    void        deleteUnusedAssets() override;

  protected:
    AssetManager&  asset_mgr;
    ContainerType* container;
};

} // namespace bembel::kernel
#include "serial-asset-loader.inl"
#endif // include guards
