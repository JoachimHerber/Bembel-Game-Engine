#ifndef BEMBEL_KERNEL_SCENE_ENTITY_HPP
#define BEMBEL_KERNEL_SCENE_ENTITY_HPP

#include "./scene.hpp"

namespace bembel::kernel {

class BEMBEL_API Entity {
  public:
    Entity(Scene*, Scene::EntityID);
    Entity(const Entity&);
    Entity(Entity&&);

    template <class ComponentType>
    bool hasComponent();

    template <class ComponentType>
    ComponentType* createComponent();

    template <class ComponentType>
    ComponentType* getComponent();

    static Entity createEntity(Scene*);

  private:
    Scene* scene;
    Scene::EntityID id;
};

template <class ComponentType>
inline bool Entity::hasComponent() {
    if(!this->scene) return false;

    const auto maks = this->scene->getEntitys()[this->id];
    return maks & ComponentType::getComponentMask() != 0;
}

template <class ComponentType>
inline ComponentType* Entity::createComponent() {
    if(!this->scene) return nullptr;

    return this->scene->createComponent<ComponentType>(this->id);
}

template <class ComponentType>
inline ComponentType* Entity::getComponent() {
    if(!this->hasComponent<ComponentType>()) return nullptr;

    auto* container = this->scene->getComponentContainer<ComponentType>();
    return container ? container->getComponent(this->id) : nullptr;
}

} // namespace bembel::kernel
#endif // include guards
