namespace bembel::kernel {

inline ComponentContainerBase::ComponentContainerBase(Scene::ComponentTypeID id)
: type_id(id)
, mask(1ull << id) {
}

inline Scene::ComponentTypeID ComponentContainerBase::getComponentTypeID() {
    return this->type_id;
}

inline Scene::ComponentMask ComponentContainerBase::getComponentMask() {
    return this->mask;
}

template <class ComponentType>
inline ComponentType* ComponentMap<ComponentType>::createComponent(Scene::EntityID entity) {
    return &(this->components[entity]);
}

template <class ComponentType>
inline bool ComponentMap<ComponentType>::createComponent(
    Scene::EntityID entity, const xml::Element* properties, AssetManager& asset_mgr) {
    ComponentType component;
    if(ComponentType::initComponent(asset_mgr, properties, &component)) {
        this->components[entity] = component;
        return true;
    }
    return false;
}

template <class ComponentType>
inline bool ComponentMap<ComponentType>::deleteComponent(Scene::EntityID entity) {
    auto it = this->components.find(entity);
    if(it != this->components.end()) {
        this->components.erase(it);
        return true;
    }
    return false;
}

template <class ComponentType>
inline std::map<Scene::EntityID, ComponentType>& ComponentMap<ComponentType>::getComponents() {
    return this->components;
}

template <class ComponentType>
inline ComponentType* ComponentMap<ComponentType>::getComponent(Scene::EntityID entity) {
    auto it = this->components.find(entity);
    if(it != this->components.end()) return &(it->second);

    return nullptr;
}

template <class ComponentType>
inline ComponentType* ComponentArray<ComponentType>::createComponent(Scene::EntityID entity) {
    if(entity >= this->components.size()) this->components.resize(entity + 1);

    return &(this->components[entity]);
}
template <class ComponentType>
inline bool ComponentArray<ComponentType>::createComponent(
    Scene::EntityID entity, const xml::Element* properties, AssetManager& asset_mgr) {
    ComponentType component;
    if(ComponentType::initComponent(asset_mgr, properties, &component)) {
        if(entity >= this->components.size()) this->components.resize(entity + 1);

        this->components[entity] = component;
        return true;
    }
    return false;
}

template <class ComponentType>
inline bool ComponentArray<ComponentType>::deleteComponent(Scene::EntityID entity) {
    return true;
}

template <class ComponentType>
inline std::vector<ComponentType>& ComponentArray<ComponentType>::getComponents() {
    return this->components;
}

template <class ComponentType>
inline ComponentType* ComponentArray<ComponentType>::getComponent(Scene::EntityID entity) {
    assert(entity < this->components.size());
    return &this->components[entity];
}

} // namespace bembel::kernel
