module;
#include <utility>
export module bembel.kernel.core:Entity;

import bembel.base;
import :Scene;
import :Components;

namespace bembel::kernel {
using bembel::base::Exeption;

export class Entity {
  public:
    Entity(/***********************/) : m_scene{nullptr}, m_id{EntityID::INVALID} {}
    Entity(Scene& scene /**********/) : m_scene{&scene}, m_id{scene.createEntity()} {}
    Entity(Scene& scene, EntityID id) : m_scene{&scene}, m_id{id} {}
    Entity(Entity const& other) = default;
    Entity(Entity /**/&& other) = default;

    Entity& operator=(Entity const& other) = default;
    Entity& operator=(Entity /**/&& other) = default;

    bool operator==(Entity other) const { return m_scene == other.m_scene && m_id == other.m_id; }
    operator bool() { return m_scene && m_id != EntityID::INVALID; }

    template <class ComponentType>
    bool has() {
        if(!m_scene || m_id == EntityID::INVALID) return false;

        return get<ComponentType>() != nullptr;
    }

    template <class ComponentType, typename... TArgs>
    bool assign(TArgs&&... args) {
        if(!m_scene || m_id == EntityID::INVALID)
            throw Exeption("Tying to acquire component for invalid entity");

        return m_scene->assignComponent<ComponentType>(m_id, std::forward<TArgs>(args)...);
    }

    template <class ComponentType>
    ComponentType* get() {
        if(!m_scene || m_id == EntityID::INVALID)
            throw Exeption("Tying to acquire component for invalid entity");

        return m_scene->getComponent<ComponentType>(m_id);
    }

    void deleteEntity() {
        if(m_id != EntityID::INVALID && m_scene) {
            m_scene->deleteEntity(m_id);
            m_id = EntityID::INVALID;
        }
    }

    EntityID getId() const { return m_id; }

  private:
    Scene*   m_scene;
    EntityID m_id;
};

} // namespace bembel::kernel
