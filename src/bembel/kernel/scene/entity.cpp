#include "./entity.hpp"

namespace bembel::kernel {

Entity::Entity(Scene* scene, Scene::EntityID id)
: scene(scene)
, id(id) {
}
Entity::Entity(const Entity& other)
: scene(other.scene)
, id(other.id) {
}
Entity::Entity(Entity&& other)
: scene(other.scene)
, id(other.id) {
}

Entity Entity::createEntity(Scene* scene) {
    return Entity(scene, scene->createEntity());
}

} // namespace bembel::kernel
