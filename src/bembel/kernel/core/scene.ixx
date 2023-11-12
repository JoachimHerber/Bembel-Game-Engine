﻿module;
#include <any>
#include <compare>
#include <filesystem>
#include <stack>
#include <vector>
#include <string_view>
export module bembel.kernel.core:Scene;

import bembel.base;
import bembel.kernel.assets;

namespace bembel::kernel {
using namespace bembel::base;

export using ComponentTypeID = u64;
export using ComponentMask   = u64;

export enum class EntityID : u64 { INVALID = ~u64(0) };

export inline std::strong_ordering operator<=>(EntityID a, EntityID b) {
    return u64(a) <=> u64(b);
}

export class ComponentContainerBase {
  public:
    ComponentContainerBase(ComponentTypeID type_id) : m_type_id{type_id}, m_mask{1ull << type_id} {}
    virtual ~ComponentContainerBase() {}

    virtual bool createComponent(EntityID, xml::Element const*) = 0;
    virtual bool deleteComponent(EntityID)                      = 0;

    ComponentTypeID getComponentTypeID() { return m_type_id; }
    ComponentMask   getComponentMask() { return m_mask; }

  private:
    ComponentTypeID m_type_id;
    ComponentMask   m_mask;
};

export template <typename T>
concept Component = true; /* requires() {
     T::COMPONENT_TYPE_NAME;
 };//*/

export class Scene {
  public:
    Scene()                        = default;
    Scene(Scene const&)            = delete;
    Scene& operator=(Scene const&) = delete;
    ~Scene();

    template <Component T, typename... TArgs>
    void registerComponentType(TArgs&&... args) {
        auto it = m_component_type_map.find(T::COMPONENT_TYPE_NAME);
        if(it != m_component_type_map.end()) return;

        auto container = std::make_unique<typename T::Container>(
            m_container.size(), this, std::forward<TArgs>(args)...
        );

        m_component_type_map.emplace(T::COMPONENT_TYPE_NAME, m_container.size());
        m_container.push_back(std::move(container));
    }

    template <Component T>
    typename T::Container* getComponentContainer() {
        auto it = m_component_type_map.find(T::COMPONENT_TYPE_NAME);
        if(it != m_component_type_map.end()) {
            return static_cast<typename T::Container*>(m_container[it->second].get());
        }
        return nullptr;
    }

    EntityID createEntity();
    EntityID createEntity(xml::Element const*);
    bool     deleteEntity(EntityID);

    bool loadScene(std::filesystem::path file_name);

    template <Component T, typename... TArgs>
    T createComponent(EntityID id, TArgs&&... args) {
        if(std::to_underlying(id) >= m_entities.size()) return {};

        auto it = m_component_type_map.find(T::COMPONENT_TYPE_NAME);
        if(it == m_component_type_map.end()) return {};

        auto container = static_cast<typename T::Container*>(m_container[it->second].get());
        if((m_entities[std::to_underlying(id)] & container->getComponentMask()) != 0) {
            if constexpr (sizeof...(TArgs) > 0)
              logWarning("Component already exisits");
            return container->getComponent(id);
        }
        m_entities[std::to_underlying(id)] |= container->getComponentMask();
        return container->createComponent(id, std::forward<TArgs>(args)...);
    }

    template <Component... T>
    bool createComponents(EntityID id) {
        return (... && !!createComponent<T>(id));
    }

    template <Component T>
    T getComponent(EntityID id) {
        if(std::to_underlying(id) >= m_entities.size()) return nullptr; // invalided entity id

        auto it = m_component_type_map.find(T::COMPONENT_TYPE_NAME);
        if(it == m_component_type_map.end()) return nullptr; // component type does not exist

        auto container = static_cast<typename T::Container*>(m_container[it->second].get());

        if((m_entities[std::to_underlying(id)] & container->getComponentMask()) == 0)
            return nullptr; // entity doesn't have a component of the requested type

        return container->getComponent(id);
    }

    std::vector<ComponentMask> const& getEntitys() const;

    bool loadAssets(std::filesystem::path file);

  private:
    using ContainerPtr = std::unique_ptr<ComponentContainerBase>;

    std::vector<ComponentMask> m_entities;
    std::stack<EntityID>       m_unused_entity_ids;

    Dictionary<ComponentTypeID> m_component_type_map;
    std::vector<ContainerPtr>   m_container;

    std::vector<Asset<std::any>> m_assets;
};

} // namespace bembel::kernel
