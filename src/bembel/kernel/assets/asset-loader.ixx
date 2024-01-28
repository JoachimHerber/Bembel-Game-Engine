module;
#include <string_view>
#include <filesystem>
export module bembel.kernel.assets:AssetLoader;

import bembel.base;

import :AssetHandle;

namespace bembel::kernel {
using namespace bembel::base;

export class AssetLoaderBase {
  public:
    AssetLoaderBase()          = default;
    virtual ~AssetLoaderBase() = default;

    virtual AssetHandle requestAsset(In<std::filesystem::path> file_name) = 0;
    virtual AssetHandle requestAsset(In<xml::Element const*> properties)  = 0;
    virtual bool        releaseAsset(In<AssetHandle> assetHandel)         = 0;
    virtual void        deleteUnusedAssets()                              = 0;
};

} // namespace bembel::kernel
