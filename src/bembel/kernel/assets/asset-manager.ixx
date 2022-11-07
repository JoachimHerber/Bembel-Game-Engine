module;
#include "bembel/pch.h"
export module bembel.kernel.assets:AssetManager;

import bembel.base;

import :AssetHandle;
import :AssetLocator;
import :AssetLoader;
import :AssetContainer;

namespace bembel::kernel {
using namespace bembel::base;

export class AssetManager final {
  public:
  public:
    AssetManager()  = default;
    ~AssetManager() = default;

    /**
     * Adds an asset.
     * The added asset will be managed by the asset manager and its
     * refernence count will be initalised to 1.
     */
    template <typename AssetType>
    AssetHandle addAsset(std::unique_ptr<AssetType> asset, std::string_view name = "") {
        auto container = getAssetContainer<AssetType>();
        if(container) {
            auto handle = container->addAsset(std::move(asset));
            container->incrementAssetRefCount(handle);
            if(!name.empty()) container->registerAssetAlias(handle, name);
            return handle;
        }
        return AssetHandle();
    }

    /**
     * Request an asset.
     * The requested asset will be loaded if it isn't loaded already.
     * Requesting an asset will increse the reference count of the asset.
     */
    template <typename AssetType>
    AssetHandle requestAsset(std::string_view file_name) {
        return requestAsset(AssetType::ASSET_TYPE_NAME, file_name);
    }
    template <typename AssetType>
    AssetHandle requestAsset(xml::Element const* properties) {
        return requestAsset(AssetType::ASSET_TYPE_NAME, properties);
    }
    AssetHandle requestAsset(std::string_view asset_type_name, std::string_view file_name) {
        auto it = m_asset_type_map.find(asset_type_name);
        if(it == m_asset_type_map.end()) return AssetHandle();

        return m_asset_loader[it->second]->requestAsset(file_name);
    }
    AssetHandle requestAsset(std::string_view asset_type_name, xml::Element const* properties) {
        auto it = m_asset_type_map.find(asset_type_name);
        if(it == m_asset_type_map.end()) return AssetHandle();

        return m_asset_loader[it->second]->requestAsset(properties);
    }

    /**
     * Releses an asset.
     * This metode will reduce the reference count of the asset and delete
     * the asset once its reference count reaches zero.
     */
    bool releaseAsset(AssetHandle asset_handle) {
        if(asset_handle.type_id >= m_asset_loader.size()) return false;

        return m_asset_loader[asset_handle.type_id]->releaseAsset(asset_handle);
    }

    void deleteUnusedAssets() {
        for(auto& loder : m_asset_loader) loder->deleteUnusedAssets();
    }

    /**
     * @return true if the asset_handle is valide.
     */
    bool isHandelValid(AssetHandle asset_handle) {
        if(asset_handle.type_id >= m_asset_container.size()) return false;

        return m_asset_container[asset_handle.type_id]->isHandelValid(asset_handle);
    }

    /**
     * @return true if the asset_handle is valide.
     */
    void incrementAssetRefCount(AssetHandle asset_handle) {
        if(asset_handle.type_id >= m_asset_container.size()) return;

        m_asset_container[asset_handle.type_id]->incrementAssetRefCount(asset_handle);
    }

    void decrementAssetRefCount(AssetHandle asset_handle) {
        if(asset_handle.type_id >= m_asset_container.size()) return;

        m_asset_container[asset_handle.type_id]->decrementAssetRefCount(asset_handle);
    }

    /**
     * Queries a handle for a named asset.
     * Does not modify the reference count of the asset.
     * @return a handle to the asset with the request name if it exist.
     */
    template <typename AssetType>
    AssetHandle getAssetHandle(std::string_view name) {
        auto container = getAssetContainer<AssetType>();
        if(container) return container->getAssetHandle(name);
        return AssetHandle();
    }

    /**
     * returns a pointer to the specified asset or a pointer to a dummy asset.
     */
    template <typename AssetType>
    AssetType* getAsset(AssetHandle handle, bool return_dummy_if_handle_invalid = true) {
        auto container = getAssetContainer<AssetType>();
        if(container) return container->getAsset(handle, return_dummy_if_handle_invalid);
        return nullptr;
    }

    template <typename AssetType, typename... TArgs>
    bool registerAssetType(TArgs... args) {
        auto it = m_asset_type_map.find(AssetType::ASSET_TYPE_NAME);
        if(it != m_asset_type_map.end()) return false; // Asset type has already been registered

        assert(m_asset_type_map.size() < 0xFFFFLU && "number of asset-types surpasses the maximum");
        u16 type_id = u16(m_asset_type_map.size());

        auto container = std::make_unique<AssetContainer<AssetType>>(type_id);
        auto loader    = std::make_unique<typename AssetType::DefaultLoaderType>(
            *this, container.get(), args...
        );

        m_asset_type_map.emplace(AssetType::ASSET_TYPE_NAME, type_id);
        m_asset_container.push_back(std::move(container));
        m_asset_loader.push_back(std::move(loader));
        return true;
    }
    template <typename AssetType, typename AssetLoaderType, typename... TArgs>
    bool registerAssetType(TArgs... args) {
        auto it = m_asset_type_map.find(AssetType::ASSET_TYPE_NAME);
        if(it != m_asset_type_map.end()) return false; // Asset type has already been registered

        assert(m_asset_type_map.size() < 0xFFFFLU && "number of asset-types surpasses the maximum");
        u16 type_id = m_asset_type_map.size();

        auto container = std::make_unique<AssetContainer<AssetType>>(type_id);
        auto loader    = std::make_unique<AssetLoaderType>(*this, container.get(), args...);

        m_asset_type_map.emplace(AssetType::ASSET_TYPE_NAME, type_id);
        m_asset_container.push_back(std::move(container));
        m_asset_loader.push_back(std::move(loader));
        return true;
    }

    AssetLocator& getAssetLocator() { return m_asset_locator; }

  private:
    template <typename AssetType>
    AssetContainer<AssetType>* getAssetContainer() {
        auto it = m_asset_type_map.find(AssetType::ASSET_TYPE_NAME);
        if(it == m_asset_type_map.end()) return nullptr;

        return static_cast<AssetContainer<AssetType>*>(m_asset_container[it->second].get());
    }

  private:
    AssetLocator    m_asset_locator;
    Dictionary<u16> m_asset_type_map;

    std::vector<std::unique_ptr<AssetContainerBase>> m_asset_container;
    std::vector<std::unique_ptr<AssetLoaderBase>>    m_asset_loader;
};

} // namespace bembel::kernel
