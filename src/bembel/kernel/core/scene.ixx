module;
#include "bembel/pch.h"
export module bembel.kernel.core:Scene;

import bembel.base;
import bembel.kernel.assets;

import :Components;

namespace bembel::kernel {
using namespace bembel::base;

export class Scene {
  public:
    Scene(AssetManager& asste_mgr) : m_asste_mgr{asste_mgr} {}
    Scene(Scene const&)            = delete;
    Scene& operator=(Scene const&) = delete;
    ~Scene();

    AssetManager& getAssetManager() { return m_asste_mgr; }

    template <Component T>
    T::Container* requestComponentContainer() {
        auto it = m_component_type_map.find(T::COMPONENT_TYPE_NAME);
        if(it != m_component_type_map.end()) {
            return static_cast<typename T::Container*>(m_container[it->second].get());
        }

        auto container         = std::make_unique<typename T::Container>(m_container.size());
        auto container_pointer = container.get();

        m_component_type_map.emplace(T::COMPONENT_TYPE_NAME, m_container.size());
        m_container.push_back(std::move(container));
        return container_pointer;
    }
    template <Component T>
    typename T::Container* getComponentContainer() {
        auto it = m_component_type_map.find(T::COMPONENT_TYPE_NAME);
        if(it != m_component_type_map.end()) {
            return static_cast<typename T::Container*>(m_container[it->second].get());
        }
        return nullptr;
    }
    template <Component T>
    void registerComponentType() {
        auto it = m_component_type_map.find(T::COMPONENT_TYPE_NAME);
        if(it != m_component_type_map.end()) return;

        auto container = std::make_unique<typename T::Container>(m_container.size());

        m_component_type_map.emplace(T::COMPONENT_TYPE_NAME, m_container.size());
        m_container.push_back(std::move(container));
    }

    EntityID createEntity();
    EntityID createEntity(xml::Element const*);
    bool     deleteEntity(EntityID);

    bool loadScene(std::filesystem::path file_name);

    template <Component T, typename... TArgs>
    T createComponent(EntityID id, TArgs&&... args) {
        if(to_underlying(id) >= m_entities.size()) return nullptr;

        auto it = m_component_type_map.find(T::COMPONENT_TYPE_NAME);
        if(it == m_component_type_map.end()) return nullptr;

        auto container = static_cast<typename T::Container*>(m_container[it->second].get());

        m_entities[to_underlying(id)] |= container->getComponentMask();
        return container->createComponent(id, std::forward<TArgs>(args)...);
    }

    template <Component T>
    T getComponent(EntityID id) {
        if(to_underlying(id) >= m_entities.size()) return nullptr; // invalided entity id

        auto it = m_component_type_map.find(T::COMPONENT_TYPE_NAME);
        if(it == m_component_type_map.end()) return nullptr; // component type does not exist

        auto container = static_cast<typename T::Container*>(m_container[it->second].get());

        if((m_entities[to_underlying(id)] & container->getComponentMask()) == 0)
            return nullptr; // entity doesn't have a component of the requested type

        return container->getComponent(id);
    }
    template <Component T>
    T acquireComponent(EntityID id) {
        if(to_underlying(id) >= m_entities.size())
            throw Exeption("Tying to acquire component for invalid entity");

        auto it = m_component_type_map.find(T::COMPONENT_TYPE_NAME);
        if(it == m_component_type_map.end()) throw Exeption("Invalid Component Type");

        auto container = static_cast<typename T::Container*>(m_container[it->second].get());

        if((m_entities[to_underlying(id)] & container->getComponentMask()) != 0)
            return container->getComponent(id);

        m_entities[to_underlying(id)] |= container->getComponentMask();
        return container->createComponent(id);
    }

    std::vector<ComponentMask> const& getEntitys() const;

    template <typename AssetType>
    AssetHandle getAssetHandle(std::string_view name) {
        return m_asste_mgr.getAssetHandle<AssetType>(name);
    }

    template <typename AssetType>
    AssetType* getAsset(AssetHandle handle, bool return_dummy_if_handle_invalid = true) {
        return m_asste_mgr.getAsset<AssetType>(handle, return_dummy_if_handle_invalid);
    }

    bool loadAssets(std::filesystem::path file);

  private:
    void loadAsset(xml::Element const*);

  private:
    using ContainerPtr = std::unique_ptr<ComponentContainerBase>;

    std::vector<ComponentMask> m_entities;
    std::stack<EntityID>       m_unused_entity_ids;

    Dictionary<ComponentTypeID> m_component_type_map;
    std::vector<ContainerPtr>   m_container;

    AssetManager&         m_asste_mgr;
    std::set<AssetHandle> m_assets;
};

} // namespace bembel::kernel
