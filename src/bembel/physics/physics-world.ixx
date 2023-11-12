module;
#include <btBulletDynamicsCommon.h>

#include <map>
#include <memory>
export module bembel.physics:World;

import bembel.base;
import bembel.kernel;

import :RigidBody;

namespace bembel::physics {
using namespace bembel::base;
using namespace bembel::kernel;

class World;

export class PhysicsComponent {
  public:
    using Container                                       = World;
    static constexpr std::string_view COMPONENT_TYPE_NAME = "Physics";

  public:
    PhysicsComponent() = default;
    PhysicsComponent(World* world, EntityID entity = EntityID::INVALID)
      : m_world{world}, m_entity{entity} {}
    PhysicsComponent(PhysicsComponent const&) = default;
    PhysicsComponent(PhysicsComponent&&)      = default;

    PhysicsComponent& operator=(PhysicsComponent const&) = default;
    PhysicsComponent& operator=(PhysicsComponent&&)      = default;

    operator bool() const { return m_world && m_entity != EntityID::INVALID; }

    RigidBody* createRigidBody(
        In<Asset<CollisionShape>> collision_shape,
        In<units::Kilogram>       mass,
        In<vec3>                  center_of_mass_offset = {0.0f, 0.0f, 0.0f}
    );

    RigidBody* getRigidBody();

  private:
    World*   m_world  = nullptr;
    EntityID m_entity = EntityID::INVALID;
};

export class World : public ComponentContainerBase {
  public:
    World(ComponentTypeID type_id, Scene* scene, vec3 gravity = {0, -9.8, 0});
    ~World() { m_rigid_bodys.clear();}

    void update(double time_since_last_update);

    Scene* getScene() { return m_scene; }

    PhysicsComponent createComponent(EntityID entity_id);
    bool             createComponent(EntityID entity_id, xml::Element const* properties) override;

    bool deleteComponent(EntityID entity_id) override;

    PhysicsComponent getComponent(EntityID entity_id);

    RigidBody* createRigidBody(
        In<EntityID>              entity,
        In<Asset<CollisionShape>> collision_shape,
        In<units::Kilogram>       mass,
        In<vec3>                  center_of_mass_offset
    );
    RigidBody* getRigidBody(In<EntityID> entity);

    btDiscreteDynamicsWorld* getWorld() { return m_world.get(); }

    EntityID rayTestFirst(In<vec3> ray_start, In<vec3> ray_end);

  private:
    Scene* m_scene;

    std::map<EntityID, std::unique_ptr<RigidBody>> m_rigid_bodys;

    std::unique_ptr<btBroadphaseInterface>               m_broadphase;
    std::unique_ptr<btDefaultCollisionConfiguration>     m_collision_config;
    std::unique_ptr<btCollisionDispatcher>               m_dispatcher;
    std::unique_ptr<btSequentialImpulseConstraintSolver> m_solver;
    std::unique_ptr<btDiscreteDynamicsWorld>             m_world;
};

} // namespace bembel::physics