module;
#include <tuple>
export module bembel.kernel.core:Entity;

import bembel.base;
import :Scene;
import :Components;

namespace bembel::kernel {
using bembel::base::Exeption;

export template <Component... TComponents>
class Entity {
  public:
    Entity(/***********************/) : m_scene{nullptr}, m_id{EntityID::INVALID} {}
    Entity(Scene& scene /**********/) : Entity{scene, scene.createEntity()} {}
    Entity(Scene& scene, EntityID id) : m_scene{&scene}, m_id{id} {
        if(m_scene && m_id != EntityID::INVALID) {
            (m_scene->acquireComponent<TComponents>(m_id), ...);
        }
    }
    Entity(Entity const& other) = default;
    Entity(Entity /**/&& other) = default;

    Entity& operator=(Entity const& other) = default;
    Entity& operator=(Entity /**/&& other) = default;

    bool operator==(Entity other) { return m_scene == other.m_scene && m_id == other.m_id; }
         operator bool() { return m_id != EntityID::INVALID; }

    template <class ComponentType>
    bool hasComponent() {
        return getComponent<ComponentType>() != nullptr;
    }

    template <class ComponentType, typename... TArgs>
    ComponentType* createComponent(TArgs&&... args) {
        return m_scene ? m_scene->createComponent<ComponentType>(m_id, std::forward<TArgs>(args)...)
                       : nullptr;
    }

    template <class ComponentType>
    ComponentType* getComponent() {
        return m_scene ? m_scene->getComponent<ComponentType>(m_id) : nullptr;
    }

    template <class ComponentType>
    ComponentType& acquireComponent() {
        if(!m_scene || m_id == EntityID::INVALID)
            throw Exeption("Tying to acquire component for invalid entity");
        return m_scene->acquireComponent<ComponentType>(m_id);
    }

    void deleteEntity() {
        if(m_id != EntityID::INVALID) {
            m_scene->deleteEntity(m_id);
            m_id = EntityID::INVALID;
        }
    }

  private:
    Scene*   m_scene;
    EntityID m_id;
};

} // namespace bembel::kernel

namespace std {
using bembel::kernel::Component;
using bembel::kernel::Entity;

export template <Component... TComponents>
struct tuple_size<Entity<TComponents...>> : integral_constant<size_t, sizeof...(TComponents)> {};

export template <size_t TIndex, Component... TComponents>
struct tuple_element<TIndex, Entity<TComponents...>>
  : tuple_element<TIndex, tuple<TComponents...>> {};

export template <size_t TIndex, Component... TComponents>
std::tuple_element_t<TIndex, tuple<TComponents...>>& get(Entity<TComponents...>& entity) {
    return *entity.getComponent<std::tuple_element_t<TIndex, tuple<TComponents...>>>();
}

} // namespace std