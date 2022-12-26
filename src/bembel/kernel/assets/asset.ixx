module;
#include <any>
#include <cassert>
#include <memory>
#include <string_view>
export module bembel.kernel.assets:Asset;

import bembel.base;

import :AssetHandle;
import :AssetLoader;
import :AssetContainer;

namespace bembel::kernel {
using namespace bembel::base;

export template <typename T>
concept AssetType = std::same_as<T, std::any> || requires(T a) {
                                                     {
                                                         T::ASSET_TYPE_NAME
                                                         } -> std::convertible_to<std::string_view>;
                                                 };

export template <AssetType T>
class Asset final {
  public:
    Asset() {}
    Asset(In<std::string_view> name) {
        if(g_container) {
            m_handel = g_container->getAssetHandle(name);
            g_container->incrementAssetRefCount(m_handel);
        }
    }
    Asset(Asset<T> const& other) : m_handel(other.m_handel) {
        if(g_container) { g_container->incrementAssetRefCount(m_handel); }
    }
    Asset(Asset<T>&& other) noexcept : m_handel(other.m_handel) { other.m_handel = AssetHandle(); }
    ~Asset() {
        if(g_loader) { g_loader->releaseAsset(m_handel); }
    }

    Asset<T>& operator=(Asset<T> const& other) {
        set(other.m_handel, true);
        return *this;
    }
    Asset<T>& operator=(Asset<T>&& other) {
        set(other.m_handel, false);
        other.m_handel = AssetHandle();
        return *this;
    }

    operator bool() const { return g_container && g_container->isHandelValid(m_handel); }

    T* operator->() const { return get(); }

    void release() {
        if(g_loader) { g_loader->releaseAsset(m_handel); }
        m_handel = AssetHandle();
    }

    T* get() const { return g_container ? g_container->getAsset(m_handel) : nullptr; }

    bool request(std::string_view file_name) {
        if(!g_loader) return false;
        AssetHandle handl = g_loader->requestAsset(file_name);
        return this->set(handl, false);
    }

    bool request(xml::Element const* properties) {
        if(!g_loader) return false;
        AssetHandle handl = g_loader->requestAsset(properties);
        return this->set(handl, false);
    }

    static void setContainer(std::unique_ptr<AssetContainer<T>> container) {
        g_container = std::move(container);
    }
    static void setLoader(std::unique_ptr<AssetLoaderBase> loader) {
        g_loader = std::move(loader);
    }

    static auto getContainer() { return g_container.get(); }
    static auto getLoader() { return g_loader.get(); }

  private:
    bool set(AssetHandle handel, bool incrementRefCount) {
        if(!g_container || !g_loader) return false;
        if(!g_container->isHandelValid(handel)) return false;

        g_loader->releaseAsset(m_handel);

        m_handel = handel;

        if(incrementRefCount) { g_container->incrementAssetRefCount(m_handel); }

        return true;
    }

  private:
    AssetHandle m_handel;

    static inline std::unique_ptr<AssetContainer<T>> g_container;
    static inline std::unique_ptr<AssetLoaderBase>   g_loader;
};

namespace assets {
    auto& getAssetTypeMap() {
        static Dictionary<std::pair<AssetContainerBase*, AssetLoaderBase*>> instance;
        return instance;
    }
    auto& getAssetTypeList() {
        static std::vector<std::pair<AssetContainerBase*, AssetLoaderBase*>> instance;
        return instance;
    }

    export template <typename TAssetType>
    bool registerAssetType() {
        auto& asset_type_map = getAssetTypeMap();
        auto  it             = asset_type_map.find(TAssetType::ASSET_TYPE_NAME);
        if(it != asset_type_map.end()) return false; // Asset type has already been registered

        assert(asset_type_map.size() < 0xFFFFLU && "number of asset-types surpasses the maximum");
        u16 type_id = asset_type_map.size();

        auto container = std::make_unique<AssetContainer<TAssetType>>(type_id);
        auto loader    = std::make_unique<TAssetType::DefaultLoaderType>(container.get());
        std::pair<AssetContainerBase*, AssetLoaderBase*> container_and_loader{
            container.get(), loader.get()};

        Asset<TAssetType>::setContainer(std::move(container));
        Asset<TAssetType>::setLoader(std::move(loader));

        asset_type_map.emplace(TAssetType::ASSET_TYPE_NAME, container_and_loader);
        getAssetTypeList().push_back(container_and_loader);
        return true;
    }
    export template <typename TAssetType, typename TLoader, typename... TArgs>
    bool registerAssetType(TArgs&&... args) {
        auto& asset_type_map = getAssetTypeMap();
        auto  it             = asset_type_map.find(TAssetType::ASSET_TYPE_NAME);
        if(it != asset_type_map.end()) return false; // Asset type has already been registered

        assert(asset_type_map.size() < 0xFFFFLU && "number of asset-types surpasses the maximum");
        u16 type_id = asset_type_map.size();

        auto      container = std::make_unique<AssetContainer<TAssetType>>(type_id);
        auto      loader = std::make_unique<TLoader>(container.get(), std::forward<TArgs>(args)...);
        std::pair container_and_loader{container.get(), loader.get()};

        Asset<TAssetType>::setContainer(std::move(container));
        Asset<TAssetType>::setLoader(std::move(loader));

        asset_type_map.emplace(TAssetType::ASSET_TYPE_NAME, container_and_loader);
        getAssetTypeList().push_back(container_and_loader);
        return true;
    }

    export void deleteUnusedAssets() {
        for(auto it : getAssetTypeMap()) { it.second.second->deleteUnusedAssets(); }
    }

} // namespace assets

export template <>
class Asset<std::any> final {
  public:
    Asset() {}
    template <AssetType T>
    Asset(Asset<T> const& other) : m_handel(other.m_handel) {
        if(Asset<T>::g_container) Asset<T>::g_container->incrementAssetRefCount(m_handel);
    }
    template <AssetType T>
    Asset(Asset<T>&& other) noexcept : m_handel(other.m_handel) {
        other.m_handel = AssetHandle();
    }
    ~Asset() {
        auto& asset_type_list = assets::getAssetTypeList();
        if(m_handel.type_id < asset_type_list.size()) {
            asset_type_list[m_handel.type_id].second->releaseAsset(m_handel);
        }
    }

    template <AssetType T>
    Asset<std::any>& operator=(Asset<T> const& other) {
        set(other.m_handel, true);
        return *this;
    }
    template <AssetType T>
    Asset<std::any>& operator=(Asset<T>&& other) {
        set(other.m_handel, false);
        other.m_handel = AssetHandle();
        return *this;
    }

    operator bool() const {
        auto& asset_type_list = assets::getAssetTypeList();
        if(m_handel.type_id < asset_type_list.size()) {
            return asset_type_list[m_handel.type_id].first->isHandelValid(m_handel);
        }
        return false;
    }

    void release() {
        auto& asset_type_list = assets::getAssetTypeList();
        if(m_handel.type_id < asset_type_list.size()) {
            asset_type_list[m_handel.type_id].second->releaseAsset(m_handel);
        }
        m_handel = AssetHandle();
    }

    bool request(xml::Element const* properties) {
        auto& asset_type_map = assets::getAssetTypeMap();
        auto  it             = asset_type_map.find(properties->Value());
        if(it == asset_type_map.end()) { return false; }
        AssetHandle handl = it->second.second->requestAsset(properties);
        return this->set(handl, false);
    }

  private:
    bool set(AssetHandle handel, bool incrementRefCount) {
        auto& asset_type_list = assets::getAssetTypeList();
        if(handel.type_id >= asset_type_list.size()) { return false; }

        if(m_handel.type_id < asset_type_list.size()) {
            asset_type_list[m_handel.type_id].second->releaseAsset(m_handel);
        }

        m_handel = handel;

        if(incrementRefCount) {
            asset_type_list[m_handel.type_id].first->incrementAssetRefCount(m_handel);
        }

        return true;
    }

  private:
    AssetHandle m_handel;
};

} // namespace bembel::kernel
