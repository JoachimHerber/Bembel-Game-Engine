module;
#include "bembel/pch.h"
export module bembel.kernel.core:Components;

import bembel.base;
import bembel.kernel.assets;

namespace bembel::kernel {
using namespace bembel::base;

export using ComponentTypeID = u64;
export using ComponentMask   = u64;

export enum class EntityID : u64 { INVALID = ~u64(0) };

export class ComponentContainerBase {
  public:
    ComponentContainerBase(ComponentTypeID type_id) : m_type_id{type_id}, m_mask{1ull << type_id} {}
    virtual ~ComponentContainerBase() {}

    virtual bool createComponent(EntityID, xml::Element const*, AssetManager&) = 0;
    virtual bool deleteComponent(EntityID)                                     = 0;

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

namespace component_utiles {
    template <typename TComponent, typename... TArgs>
    concept StaticInitializableFrom = requires(TComponent c, TArgs... args) {
        { TComponent::initComponent(args..., c) } -> std::convertible_to<bool>;
    };

    template <Component T>
    bool initComponent(
        In<xml::Element const*> properties,
        InOut<AssetManager>     asset_mgr,
        InOut<T>                geometry_component
    ) {
        if constexpr(StaticInitializableFrom<T, xml::Element const*, AssetManager>) {
            return T::initComponent(properties, asset_mgr, geometry_component);
        } else if constexpr(StaticInitializableFrom<T, xml::Element const*>) {
            return T::initComponent(properties, geometry_component);
        } else {
            return true;
        }
    }
} // namespace component_utiles

export template <Component T>
class ComponentMap : public ComponentContainerBase {
  public:
    ComponentMap(ComponentTypeID type_id) : ComponentContainerBase(type_id) {}
    virtual ~ComponentMap() {}

    T* createComponent(EntityID entity_id) { return &(m_components[entity_id]); }
    template <typename... TArgs>
    T* createComponent(EntityID entity_id, TArgs&&... args) {
        return new(&(m_components[entity_id])) T(std::forward<TArgs>(args)...);
    }
    bool createComponent(
        EntityID entity_id, base::xml::Element const* properties, AssetManager& asset_mgr
    ) override {
        T component;
        if(component_utiles::initComponent(properties, asset_mgr, component)) {
            m_components[entity_id] = component;
            return true;
        }
        return false;
    }
    bool deleteComponent(EntityID entity_id) override {
        auto it = m_components.find(entity_id);
        if(it != m_components.end()) {
            m_components.erase(it);
            return true;
        }
        return false;
    }

    std::map<EntityID, T>& getComponents() { return m_components; }

    T* getComponent(EntityID entity_id) {
        auto it = m_components.find(entity_id);
        if(it != m_components.end()) return &(it->second);

        return nullptr;
    }

  private:
    std::map<EntityID, T> m_components;
};

export template <Component T>
class ComponentArray : public ComponentContainerBase {
  public:
    ComponentArray(ComponentTypeID id) : ComponentContainerBase(id) {}
    virtual ~ComponentArray() {}

    T* createComponent(EntityID entity_id) {
        if(to_underlying(entity_id) >= m_components.size())
            m_components.resize(to_underlying(entity_id) + 1);

        return &(m_components[to_underlying(entity_id)]);
    }
    template <typename... TArgs>
    T* createComponent(EntityID entity_id, TArgs&&... args) {
        if(to_underlying(entity_id) >= m_components.size())
            m_components.resize(to_underlying(entity_id) + 1);

        return new(&(m_components[to_underlying(entity_id)])) T(std::forward<TArgs>(args)...);
    }
    bool createComponent(
        EntityID entity_id, xml::Element const* properties, AssetManager& asset_mgr
    ) override {
        T component;
        if(component_utiles::initComponent(properties, asset_mgr, component)) {
            if(to_underlying(entity_id) >= m_components.size())
                m_components.resize(to_underlying(entity_id) + 1);

            m_components[to_underlying(entity_id)] = component;
            return true;
        }
        return false;
    }
    bool deleteComponent(EntityID entity_id) override { return true; }

    std::vector<T>& getComponents() { return m_components; }
    T*              getComponent(EntityID entity_id) {
                     assert(to_underlying(entity_id) < m_components.size());
                     return &m_components[to_underlying(entity_id)];
    }

  private:
    std::vector<T> m_components;
};

} // namespace bembel::kernel
