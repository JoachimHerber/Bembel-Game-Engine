module;
#include "bembel/pch.h"
export module bembel.kernel.assets:AssetLoader;

import bembel.base;

import :AssetHandle;

namespace bembel::kernel {
using namespace bembel::base;

export class AssetLoaderBase {
  public:
    AssetLoaderBase()          = default;
    virtual ~AssetLoaderBase() = default;

    virtual AssetHandle requestAsset(std::string_view file_name)     = 0;
    virtual AssetHandle requestAsset(xml::Element const* properties) = 0;
    virtual bool        releaseAsset(AssetHandle assetHandel)        = 0;
    virtual void        deleteUnusedAssets()                         = 0;
};

} // namespace bembel::kernel
