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

export template <StringLiteral TTypeName, typename TDataType, bool TDense = true>
class StandardComponent {
  public:
    StandardComponent(TDataType* data = nullptr) : m_data{data} {}
    StandardComponent(StandardComponent const&) = default;
    StandardComponent(StandardComponent&&)      = default;

    StandardComponent& operator=(StandardComponent const&) = default;
    StandardComponent& operator=(StandardComponent&&)      = default;
    StandardComponent& operator=(In<TDataType> data) {
        *m_data = data;
        return *this;
    }

    operator bool() const { return m_data != nullptr; }

    bool operator==(StandardComponent other) { return *m_data == *other.m_data; }
    bool operator==(In<TDataType> data) { return *m_data == data; }

    auto& operator*() { return *m_data; }
    auto* operator->() { return m_data; }

    static constexpr std::string_view COMPONENT_TYPE_NAME = TTypeName.value;

    class Container : public ComponentContainerBase {
      public:
        using ContainerType =
            std::conditional_t<TDense, std::vector<TDataType>, std::map<EntityID, TDataType>>;

        Container(ComponentTypeID type_id) : ComponentContainerBase(type_id) {}
        virtual ~Container() = default;

        StandardComponent createComponent(EntityID entity_id) {
            if constexpr(TDense) {
                if(to_underlying(entity_id) >= m_components.size())
                    m_components.resize(to_underlying(entity_id) + 1);
                return {&(m_components[to_underlying(entity_id)])};
            } else {
                return {&(m_components[entity_id])};
            }
        }

        template <typename... TArgs>
        StandardComponent createComponent(EntityID entity_id, TArgs&&... args) {
            auto component = createComponent(entity_id);
            *component     = TDataType(std::forward<TArgs>(args)...);
            return component;
        }

        bool createComponent(
            EntityID entity_id, xml::Element const* properties, AssetManager& asset_mgr
        ) override {
            TDataType component;
            if(initComponent(properties, asset_mgr, component)) {
                if constexpr(TDense) {
                    if(to_underlying(entity_id) >= m_components.size())
                        m_components.resize(to_underlying(entity_id) + 1);
                    m_components[to_underlying(entity_id)] = component;
                } else {
                    m_components[entity_id] = component;
                }
                return true;
            }
            return false;
        }

        bool deleteComponent(EntityID entity_id) override {
            if constexpr(TDense) {
                return true;
            } else {
                auto it = m_components.find(entity_id);
                if(it != m_components.end()) {
                    m_components.erase(it);
                    return true;
                }
                return false;
            }
        }

        auto& getComponentData() { return m_components; }

        StandardComponent getComponent(EntityID entity_id) {
            if constexpr(TDense) {
                assert(to_underlying(entity_id) < m_components.size());
                return &m_components[to_underlying(entity_id)];
            } else {
                auto it = m_components.find(entity_id);
                if(it != m_components.end()) return {&(it->second)};
            }
            return {nullptr};
        }

      private:
        ContainerType m_components;
    };

  private:
    TDataType* m_data;
};

} // namespace bembel::kernel
