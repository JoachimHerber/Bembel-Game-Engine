module;
#include <any>
#include <compare>
#include <filesystem>
#include <stack>
#include <string_view>
#include <type_traits>
#include <typeindex>
#include <unordered_map>
#include <vector>
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

    virtual bool serializeComponent(EntityID, xml::Element*)         = 0;
    virtual bool deserializeComponent(EntityID, xml::Element const*) = 0;

    virtual bool deleteComponent(EntityID) = 0;

    ComponentTypeID getComponentTypeID() { return m_type_id; }
    ComponentMask   getComponentMask() { return m_mask; }

  private:
    ComponentTypeID m_type_id;
    ComponentMask   m_mask;
};

export class SceneDataContainerBase {
  public:
    SceneDataContainerBase()          = default;
    virtual ~SceneDataContainerBase() = default;
};

export template <typename T>
struct ComponentMetaData;

export template <typename T>
    requires requires() {
        typename T::Container;
        requires std::is_base_of_v<ComponentContainerBase, typename T::Container>;
    }
struct ComponentMetaData<T> {
    using Container                                              = T::Container;
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
        auto it = m_component_containers.find(std::type_index(typeid(T)));
        if(it != m_component_containers.end()) return;

        m_component_containers.emplace(
            std::type_index(typeid(T)),
            std::make_unique<typename ComponentMetaData<T>::Container>(
                m_component_containers.size(), this, std::forward<TArgs>(args)...
            )
        );
    }

    template <Component T>
    typename ComponentMetaData<T>::Container* getComponentContainer() {
        auto it = m_component_containers.find(std::type_index(typeid(T)));
        if(it != m_component_containers.end()) {
            return static_cast<typename ComponentMetaData<T>::Container*>(it->second.get());
        }
        return nullptr; // component type does not exist
    }

    EntityID createEntity();
    EntityID createEntity(xml::Element const*);
    bool     deleteEntity(EntityID);

    bool loadScene(std::filesystem::path file_name);

    template <Component T, typename... TArgs>
    bool assignComponent(EntityID id, TArgs&&... args) {
        if(std::to_underlying(id) >= m_entities.size()) return false;

        auto* container = getComponentContainer<T>();
        if(!container) return false;

        container->assignComponent(id, std::forward<TArgs>(args)...);
        m_entities[std::to_underlying(id)] |= container->getComponentMask();
        return true;
    }

    template <Component T>
    T* getComponent(EntityID id) {
        if(std::to_underlying(id) >= m_entities.size()) return nullptr;

        auto* container = getComponentContainer<T>();
        if(!container) return nullptr;

        if((m_entities[std::to_underlying(id)] & container->getComponentMask()) == 0)
            return nullptr; // entity doesn't have a component of the requested type

        return container->getComponent(id);
    }

    std::vector<ComponentMask> const& getEntitys() const;

    bool loadAssets(std::filesystem::path file);

    template <typename T>
    T* getDataContainer()
        requires std::is_base_of_v<SceneDataContainerBase, T>
    {
        auto it = m_data_containers.find(std::type_index(typeid(T)));
        if(it != m_data_containers.end()) return static_cast<T*>(it->second.get());
        return nullptr;
    }

    template <typename T, typename... TArgs>
    bool createDataContainer(TArgs&&... args)
        requires std::is_base_of_v<SceneDataContainerBase, T>
    {
        auto it = m_data_containers.find(std::type_index(typeid(T)));
        if(it != m_data_containers.end()) return false;

        m_data_containers.emplace(
            std::type_index(typeid(T)), std::make_unique<T>(this, std::forward<TArgs>(args)...)
        );
        return true;
    }

  private:
    using ContainerPtr = std::unique_ptr<ComponentContainerBase>;

    std::vector<ComponentMask> m_entities;
    std::stack<EntityID>       m_unused_entity_ids;

    template <typename T>
    using TypeMap = std::unordered_map<std::type_index, std::unique_ptr<T>>;

    TypeMap<ComponentContainerBase> m_component_containers;
    TypeMap<SceneDataContainerBase> m_data_containers;

    std::vector<Asset<std::any>> m_assets;
};

} // namespace bembel::kernel
