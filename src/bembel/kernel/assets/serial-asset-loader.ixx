module;
#include <filesystem>
#include <string_view>
export module bembel.kernel.assets:SerialAssetLoader;

import :AssetLoader;
import :AssetContainer;
import :AssetLocator;

namespace bembel::kernel {
using namespace bembel::base;

export template <typename T>
concept SerialLoadableAsset = true;
// requires(
//     T                     a,
//     AssetManager&         asset_mgr,
//     std::filesystem::path path,
//     xml::Element const*   properties,
//     std::unique_ptr<T>    ptr
//) {
//     { T::ASSET_TYPE_NAME /*************************/ } -> std::convertible_to<std::string_view>;
//     { T::loadAsset(asset_mgr, path) /**************/ } -> std::same_as<std::unique_ptr<T>>;
//     { T::createAsset(asset_mgr, properties) /******/ } -> std::same_as<std::unique_ptr<T>>;
//     { T::deleteAsset(asset_mgr, std::move(ptr)) /**/ } -> std::same_as<void>;
// };

template <typename AssetType>
concept HasStaticLoadAssetMethode = requires(std::filesystem::path p) {
    { AssetType::loadAsset(p) } -> std::convertible_to<std::unique_ptr<AssetType>>;
};

export template <SerialLoadableAsset AssetType>
class SerialAssetLoader : public AssetLoaderBase {
  public:
    using ContainerType = AssetContainer<AssetType>;

    SerialAssetLoader(ContainerType* container) : m_container(container) {}
    virtual ~SerialAssetLoader() = default;

    AssetHandle requestAsset(In<std::filesystem::path> file_name) override {
        // test if the asset is alredy loaded
        AssetHandle handle = m_container->getAssetHandle(file_name.string());
        if(m_container->isHandelValid(handle)) {
            m_container->incrementAssetRefCount(handle);
            return handle;
        }

        auto path = AssetLocator::getInstance().findAssetLocation<AssetType>(file_name);
        if(!path) {
            logWarning("can't find location of '{}'", file_name.string());
            // can't find the requested file
            return AssetHandle();
        }

        // we have to load the asset
        std::unique_ptr<AssetType> asset;
        if constexpr(HasStaticLoadAssetMethode<AssetType>) {
            asset = AssetType::loadAsset(path.value());
        } else {
            std::string const file_path = path.value().string(); // file.c_str() returns a wchar*
            xml::Document     doc;
            if(doc.LoadFile(file_path.c_str()) != tinyxml2::XML_SUCCESS) {
                logError("Failed to load file '{}' \n {}", file_path, doc.ErrorName());
            } else {
                xml::Element const* root = doc.FirstChildElement(AssetType::ASSET_TYPE_NAME.data());
                if(!root) {
                    logError(
                        "File '{}' has no root element '{}'", file_path, AssetType::ASSET_TYPE_NAME
                    );
                } else {
                    asset = AssetType::createAsset(root);
                }
            }
        }
        if(!asset) return AssetHandle();

        handle = m_container->addAsset(std::move(asset));
        m_container->registerAssetAlias(handle, file_name.string());

        m_container->incrementAssetRefCount(handle);
        return handle;
    }

    AssetHandle requestAsset(xml::Element const* properties) override {
        if(!properties) {
            logWarning("Can't request Asset: properties == nullptr");
            return AssetHandle();
        }

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
            AssetHandle handle = requestAsset(std::filesystem::path(file_name));
            if(m_container->isHandelValid(handle) && !name.empty()) {
                m_container->registerAssetAlias(handle, name);
            }
            return handle;
        }

        std::unique_ptr<AssetType> asset = AssetType::createAsset(properties);
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
                m_container->removeAsset(asset_handel);
                return true;
            }
        }
        return false;
    }

    void deleteUnusedAssets() override {
        std::vector<AssetHandle> unusedAssets;
        m_container->getUnusedAssets(unusedAssets);
        for(auto hndl : unusedAssets) { m_container->removeAsset(hndl); }
    }

  protected:
    ContainerType* m_container;
};

} // namespace bembel::kernel
