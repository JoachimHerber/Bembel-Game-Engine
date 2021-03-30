#ifndef BEMBEL_KERNEL_ASSETS_ASSETLOADER_HPP
#define BEMBEL_KERNEL_ASSETS_ASSETLOADER_HPP

#include <bembel/base/io/xml.hpp>

#include "./asset-handle.hpp"

namespace bembel::kernel {

class AssetManager;
class AssetContainerBase;

class BEMBEL_API AssetLoaderBase {
  public:
    AssetLoaderBase()          = default;
    virtual ~AssetLoaderBase() = default;

    virtual AssetHandle requestAsset(const std::string& file_name)         = 0;
    virtual AssetHandle requestAsset(const base::xml::Element* properties) = 0;
    virtual bool        releaseAsset(AssetHandle assetHandel)              = 0;
    virtual void        deleteUnusedAssets()                               = 0;
};

} // namespace bembel::kernel
#endif // include guards
