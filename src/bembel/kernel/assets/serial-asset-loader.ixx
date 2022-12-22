module;
#include <string_view>
export module bembel.kernel.assets:SerialAssetLoader;

import :AssetLoader;
import :AssetContainer;
import :AssetManager;

namespace bembel::kernel {
using namespace bembel::base;

export template <typename T>
concept SerialLoadableAsset = true;
//requires(
//    T                     a,
//    AssetManager&         asset_mgr,
//    std::filesystem::path path,
//    xml::Element const*   properties,
//    std::unique_ptr<T>    ptr
//) {
//    { T::ASSET_TYPE_NAME /*************************/ } -> std::convertible_to<std::string_view>;
//    { T::loadAsset(asset_mgr, path) /**************/ } -> std::same_as<std::unique_ptr<T>>;
//    { T::createAsset(asset_mgr, properties) /******/ } -> std::same_as<std::unique_ptr<T>>;
//    { T::deleteAsset(asset_mgr, std::move(ptr)) /**/ } -> std::same_as<void>;
//};

export template <SerialLoadableAsset AssetType>
class SerialAssetLoader : public AssetLoaderBase {
  public:
    using ContainerType = AssetContainer<AssetType>;

    SerialAssetLoader(AssetManager& asset_mgr, ContainerType* container)
      : m_asset_mgr(asset_mgr)
      , m_container(container) {}
    virtual ~SerialAssetLoader() = default;

    AssetHandle requestAsset(std::string_view file_name) override {
        std::string path;

        auto& locator = m_asset_mgr.getAssetLocator();
        if(!locator.findAssetLocation<AssetType>(file_name, &path)) {
            log().warning("can't find location of '{}'", file_name);
            // can't find the requested file
            return AssetHandle();
        }

        // test if the asset is alredy loaded
        AssetHandle handle = m_container->getAssetHandle(path);
        if(!m_container->isHandelValid(handle)) {
            // we have to load the asset
            std::unique_ptr<AssetType> asset = AssetType::loadAsset(m_asset_mgr, path);
            if(!asset) return AssetHandle();

            handle = m_container->addAsset(std::move(asset));
            m_container->registerAssetAlias(handle, path);
        }

        m_container->incrementAssetRefCount(handle);
        return handle;
    }

    AssetHandle requestAsset(xml::Element const* properties) override {
        if(!properties) return AssetHandle();

        std::string name = "";
        if(xml::getAttribute(properties, "name", name)) {
            AssetHandle handle = m_container->getAssetHandle(name);
            if(m_container->isHandelValid(handle)) {
                m_container->incrementAssetRefCount(handle);
                return handle;
            }
        }

        std::string file_name = "";
        if(xml::getAttribute(properties, "file_name", file_name)) {
            AssetHandle handle = requestAsset(file_name);
            if(m_container->isHandelValid(handle) && !name.empty()) {
                m_container->registerAssetAlias(handle, name);
            }
            return handle;
        }

        std::unique_ptr<AssetType> asset = AssetType::createAsset(m_asset_mgr, properties);
        if(!asset) return AssetHandle();

        AssetHandle handle = m_container->addAsset(std::move(asset));
        m_container->incrementAssetRefCount(handle);
        if(!name.empty()) m_container->registerAssetAlias(handle, name);

        return handle;
    }

    bool releaseAsset(AssetHandle asset_handel) override {
        if(m_container->isHandelValid(asset_handel)) {
            m_container->decrementAssetRefCount(asset_handel);
            if(m_container->getAssetRefCount(asset_handel) == 0) {
                AssetType::deleteAsset(m_asset_mgr, m_container->removeAsset(asset_handel));
                return true;
            }
        }
        return false;
    }

    void deleteUnusedAssets() override {
        std::vector<AssetHandle> unusedAssets;
        m_container->getUnusedAssets(unusedAssets);
        for(auto hndl : unusedAssets) {
            AssetType::deleteAsset(m_asset_mgr, m_container->removeAsset(hndl));
        }
    }

  protected:
    AssetManager&  m_asset_mgr;
    ContainerType* m_container;
};

} // namespace bembel::kernel
