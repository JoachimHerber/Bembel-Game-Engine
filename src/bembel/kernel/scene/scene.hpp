#ifndef BEMBEL_KERNEL_SCENE_SCENE_HPP
#define BEMBEL_KERNEL_SCENE_SCENE_HPP

#include <bembel/base/io/xml.hpp>
#include <bembel/kernel/assets/asset-handle.hpp>

#include "../assets/asset-manager.hpp"

namespace bembel::kernel {

class ComponentContainerBase;

class BEMBEL_API Scene {
  public:
    using EntityID        = size_t;
    using ComponentTypeID = size_t;
    using ComponentMask   = unsigned long long;

    enum { INVALID_ENTITY = ~EntityID(0) };

    Scene(AssetManager&);
    Scene(const Scene&) = delete;
    Scene& operator=(const Scene&) = delete;
    ~Scene();

    template <class ComponentType>
    typename ComponentType::ContainerType* requestComponentContainer();
    template <class ComponentType>
    typename ComponentType::ContainerType* getComponentContainer();

    template <class ComponentType>
    void registerComponentType();

    EntityID createEntity();
    EntityID createEntity(const xml::Element*);
    bool     deleteEntity(EntityID);

    bool loadScene(const std::string& file_name);

    template <class ComponentType>
    ComponentType* createComponent(EntityID id);

    template <class ComponentType>
    ComponentType* getComponent(EntityID id);

    const std::vector<ComponentMask>& getEntitys() const;

    template <typename AssetType>
    AssetHandle getAssetHandle(const std::string& name);

    template <typename AssetType>
    AssetType* getAsset(AssetHandle handle, bool return_dummy_if_handle_invalid = true);

    bool loadAssets(const std::string& file);

  private:
    void loadAsset(const xml::Element*);

  private:
    using ContainerPtr = std::unique_ptr<ComponentContainerBase>;

    std::vector<ComponentMask> entities;
    std::stack<EntityID>       unused_entity_ids;

    std::map<std::string, ComponentTypeID> component_type_map;
    std::vector<ContainerPtr>              container;

    AssetManager& asste_mgr;

    std::set<AssetHandle> assets;
};

template <class ComponentType>
inline typename ComponentType::ContainerType* Scene::requestComponentContainer() {
    auto it = this->component_type_map.find(ComponentType::getComponentTypeName());
    if(it != this->component_type_map.end()) {
        return static_cast<typename ComponentType::ContainerType*>(
            this->container[it->second].get());
    }

    auto container =
        std::make_unique<typename ComponentType::ContainerType>(this->container.size());
    auto container_pointer = container.get();

    this->component_type_map.emplace(ComponentType::getComponentTypeName(), this->container.size());
    this->container.push_back(std::move(container));
    return container_pointer;
}

template <class ComponentType>
inline typename ComponentType::ContainerType* Scene::getComponentContainer() {
    auto it = this->component_type_map.find(ComponentType::getComponentTypeName());
    if(it != this->component_type_map.end()) {
        return static_cast<typename ComponentType::ContainerType*>(
            this->container[it->second].get());
    }
    return nullptr;
}

template <class ComponentType>
inline void Scene::registerComponentType() {
    auto it = this->component_type_map.find(ComponentType::getComponentTypeName());
    if(it != this->component_type_map.end()) return;

    auto container =
        std::make_unique<typename ComponentType::ContainerType>(this->container.size());

    this->component_type_map.emplace(ComponentType::getComponentTypeName(), this->container.size());
    this->container.push_back(std::move(container));
}

template <class ComponentType>
ComponentType* Scene::createComponent(EntityID id) {
    if(id >= this->entities.size()) return nullptr;

    auto it = this->component_type_map.find(ComponentType::getComponentTypeName());
    if(it == this->component_type_map.end()) return nullptr;

    auto container =
        static_cast<typename ComponentType::ContainerType*>(this->container[it->second].get());

    this->entities[id] |= container->getComponentMask();
    return container->createComponent(id);
}

template <class ComponentType>
ComponentType* Scene::getComponent(EntityID id) {
    if(id >= this->entities.size()) return nullptr; // invalided entity id

    auto it = this->component_type_map.find(ComponentType::getComponentTypeName());
    if(it == this->component_type_map.end()) return nullptr; // component type does not exist

    auto container =
        static_cast<typename ComponentType::ContainerType*>(this->container[it->second].get());

    if((this->entities[id] & container->getComponentMask()) == 0)
        return nullptr; // entity doesn't have a component of the requested type

    return container->getComponent(id);
}

template <typename AssetType>
inline AssetHandle Scene::getAssetHandle(const std::string& name) {
    return this->asste_mgr.getAssetHandle<AssetType>(name);
}

template <typename AssetType>
inline AssetType* Scene::getAsset(AssetHandle handle, bool returnDummyIfHandleInvalid) {
    return this->asste_mgr.getAsset<AssetType>(handle, returnDummyIfHandleInvalid);
}

} // namespace bembel::kernel
#endif // include guards
