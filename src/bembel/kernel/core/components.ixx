module;
#include <bit>
#include <cassert>
#include <span>
#include <string_view>
#include <utility>
export module bembel.kernel.core:Components;

import bembel.base;
import bembel.kernel.assets;

import :Scene;

namespace bembel::kernel {
using namespace bembel::base;

export template <typename T>
class ComponentVector : public ComponentContainerBase {
  public:
    ComponentVector(ComponentTypeID type_id, Scene* scene)
      : ComponentContainerBase{type_id}, m_scene{scene} {}
    ~ComponentVector() = default;

    T* createComponent(EntityID entity_id) {
        if(std::to_underlying(entity_id) >= m_components.size()) {
            m_components.resize(std::to_underlying(entity_id) + 1);
        }
        return &(m_components[std::to_underlying(entity_id)]);
    }

    template <typename... TArgs>
    T* createComponent(EntityID entity_id, TArgs&&... args) {
        T* component = createComponent(entity_id);
        *component   = T(std::forward<TArgs>(args)...);
        return component;
    }

    bool createComponent(EntityID entity_id, xml::Element const* properties) override {
        T component;
        if(initComponent(properties, component)) {
            if(std::to_underlying(entity_id) >= m_components.size()) {
                m_components.resize(std::to_underlying(entity_id) + 1);
            }
            m_components[std::to_underlying(entity_id)] = component;
            return true;
        }
        return false;
    }

    bool deleteComponent(EntityID entity_id) override { return true; }

    auto& getComponentData() { return m_components; }

    T* getComponent(EntityID entity_id) {
        if(std::to_underlying(entity_id) < m_components.size())
            return &m_components[std::to_underlying(entity_id)];
        return nullptr;
    }

    auto begin() { return m_components.begin(); }
    auto end() { return m_components.end(); }

  private:
    Scene*         m_scene;
    std::vector<T> m_components;
};

export template <typename T>
class ComponentMap : public ComponentContainerBase {
  public:
    ComponentMap(ComponentTypeID type_id, Scene* scene)
      : ComponentContainerBase{type_id}, m_scene{scene} {}
    ~ComponentMap() = default;

    T* createComponent(EntityID entity_id) { return &(m_components[entity_id]); }

    template <typename... TArgs>
    T* createComponent(EntityID entity_id, TArgs&&... args) {
        T* component = createComponent(entity_id);
        *component   = T(std::forward<TArgs>(args)...);
        return component;
    }

    bool createComponent(EntityID entity_id, xml::Element const* properties) override {
        T component;
        if(initComponent(properties, component)) {
            m_components[entity_id] = std::move(component);
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

    auto& getComponentData() { return m_components; }

    T* getComponent(EntityID entity_id) {
        auto it = m_components.find(entity_id);
        if(it != m_components.end()) return &(it->second);
        return {nullptr};
    }

  private:
    Scene*                m_scene;
    std::map<EntityID, T> m_components;
};

export template <typename T>
class FixedAddressComponentVector : public ComponentContainerBase {
  public:
    static constexpr u64 ALLOCATION_GRANULARITY = 64 * 1024;
    static constexpr u64 COMPONENTS_PER_CHUNK   = ALLOCATION_GRANULARITY / sizeof(T);

    static constexpr bool USE_BIT_MASK          = std::has_single_bit(COMPONENTS_PER_CHUNK);
    static constexpr bool USE_STD_ALIGNED_ALLOC = false;

    using Chunk = std::span<T, COMPONENTS_PER_CHUNK>;

    struct Iterator {
        std::vector<Chunk>::iterator chunk;
        u64                          index = 0;

        T& operator*() { return (*chunk)[index]; }
        T* operator->() { return &((*chunk)[index]); }
        operator bool() { return chunk; }

        Iterator& operator++() {
            if((++index) == COMPONENTS_PER_CHUNK) {
                index = 0;
                ++chunk;
            }
            return *this;
        }
        Iterator operator++(int) {
            Iterator tmp = *this;
            return ++tmp;
        }
    };

  public:
    FixedAddressComponentVector(ComponentTypeID type_id, Scene* scene)
      : ComponentContainerBase{type_id}, m_scene{scene} {}
    ~FixedAddressComponentVector() {
        for(Chunk it : m_components) { memory::free(it); }
    }
    T& operator[](EntityID entity_id) {
        auto [chunk, index] = getLocation(entity_id);
        while(chunk >= m_components.size()) { m_components.push_back(memory::alloc<T>()); }
        return m_components[chunk][index];
    }

    template <typename... TArgs>
    T* createComponent(EntityID entity_id, TArgs&&... args) {
        auto* component = &operator[](entity_id);
        new(component) T(std::forward<TArgs>(args)...);
        return {component};
    }

    bool createComponent(EntityID entity_id, xml::Element const* properties) override {
        T component;
        if(initComponent(properties, component)) {
            operator[](entity_id) = component;
            return true;
        }
        return false;
    }

    bool deleteComponent(EntityID entity_id) override { return true; }

    Iterator begin() { return {m_components.begin(), 0}; }
    Iterator end() { return {m_components.end(), 0}; }

    T* getComponent(EntityID entity_id) {
        auto [chunk, index] = getLocation(entity_id);
        if(chunk < m_components.size()) return {&(m_components[chunk][index])};
        return {nullptr};
    }

  private:
    std::pair<u64, u64> getLocation(EntityID entity_id) {
        if constexpr(USE_BIT_MASK) {
            constexpr u64 BIT_MASK  = COMPONENTS_PER_CHUNK - 1;
            constexpr u64 BIT_SHIFT = std::bit_width(COMPONENTS_PER_CHUNK);
            return {
                std::to_underlying(entity_id) >> BIT_SHIFT,
                std::to_underlying(entity_id) & BIT_MASK};
        } else {
            return {
                std::to_underlying(entity_id) / COMPONENTS_PER_CHUNK,
                std::to_underlying(entity_id) % COMPONENTS_PER_CHUNK};
        }
    }

  private:
    Scene*             m_scene;
    std::vector<Chunk> m_components;
};

export enum class ComponentContainer { VECTOR, MAP, FIXED_ADDRESS_VECTOR };

export template <
    StringLiteral TTypeName,
    typename TDataType,
    ComponentContainer TContainer = ComponentContainer::VECTOR>

struct BasicComponentMetaData {
    static constexpr std::string_view COMPONENT_TYPE_NAME = TTypeName.value;

    using Container = std::tuple_element_t<
        std::to_underlying(TContainer),
        std::tuple<
            ComponentVector<TDataType>,
            ComponentMap<TDataType>,
            FixedAddressComponentVector<TDataType>>>;
};

} // namespace bembel::kernel
