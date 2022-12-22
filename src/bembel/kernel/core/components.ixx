module;
#include <cassert>
#include <string_view>
#include <utility>
export module bembel.kernel.core:Components;

import bembel.base;
import bembel.kernel.assets;

import :Scene;

namespace bembel::kernel {
using namespace bembel::base;

export template <typename TComponentType, typename TDataType, bool TDense = true>
class DefaultComponentContainer : public ComponentContainerBase {
  public:
    using ContainerType =
        std::conditional_t<TDense, std::vector<TDataType>, std::map<EntityID, TDataType>>;

    DefaultComponentContainer(ComponentTypeID type_id, Scene* scene)
      : ComponentContainerBase{type_id}, m_scene{scene} {}
    ~DefaultComponentContainer() = default;

    TComponentType createComponent(EntityID entity_id) {
        if constexpr(TDense) {
            if(std::to_underlying(entity_id) >= m_components.size())
                m_components.resize(std::to_underlying(entity_id) + 1);
            return {&(m_components[std::to_underlying(entity_id)])};
        } else {
            return {&(m_components[entity_id])};
        }
    }

    template <typename... TArgs>
    TComponentType createComponent(EntityID entity_id, TArgs&&... args) {
        auto component = createComponent(entity_id);
        *component     = TDataType(std::forward<TArgs>(args)...);
        return component;
    }

    bool createComponent(EntityID entity_id, xml::Element const* properties) override {
        TDataType component;
        if(initComponent(properties, m_scene->getAssetManager(), component)) {
            if constexpr(TDense) {
                if(std::to_underlying(entity_id) >= m_components.size())
                    m_components.resize(std::to_underlying(entity_id) + 1);
                m_components[std::to_underlying(entity_id)] = component;
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

    TComponentType getComponent(EntityID entity_id) {
        if constexpr(TDense) {
            assert(std::to_underlying(entity_id) < m_components.size());
            return &m_components[std::to_underlying(entity_id)];
        } else {
            auto it = m_components.find(entity_id);
            if(it != m_components.end()) return {&(it->second)};
        }
        return {nullptr};
    }

  private:
    Scene*        m_scene;
    ContainerType m_components;
};

export template <StringLiteral TTypeName, typename TDataType, bool TDense = true>
class BasicComponent {
  public:
    BasicComponent(TDataType* data = nullptr) : m_data{data} {}
    BasicComponent(BasicComponent const&) = default;
    BasicComponent(BasicComponent&&)      = default;
    ~BasicComponent()                     = default;

    BasicComponent& operator=(BasicComponent const&) = default;
    BasicComponent& operator=(BasicComponent&&)      = default;
    BasicComponent& operator=(In<TDataType> data) {
        *m_data = data;
        return *this;
    }

    operator bool() const { return m_data != nullptr; }

    bool operator==(BasicComponent other) const { return *m_data == *other.m_data; }
    bool operator==(In<TDataType> data) const { return *m_data == data; }

    auto& operator*() { return *m_data; }
    auto* operator->() { return m_data; }

    static constexpr std::string_view COMPONENT_TYPE_NAME = TTypeName.value;

    using Container = DefaultComponentContainer<BasicComponent, TDataType, TDense>;

  private:
    TDataType* m_data;
};

} // namespace bembel::kernel
