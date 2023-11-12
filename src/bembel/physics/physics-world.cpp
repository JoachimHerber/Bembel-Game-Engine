module;
#include <btBulletDynamicsCommon.h>

#include <memory>
#include <string>
module bembel.physics;

import bembel.base;
import bembel.kernel;

namespace bembel::physics {
using namespace bembel::base;
using namespace bembel::kernel;

RigidBody* PhysicsComponent::createRigidBody(
    In<Asset<CollisionShape>> collision_shape,
    In<units::Kilogram>       mass,
    In<vec3>                  center_of_mass_offset
) {
    return m_world
             ? m_world->createRigidBody(m_entity, collision_shape, mass, center_of_mass_offset)
             : nullptr;
}
RigidBody* PhysicsComponent::getRigidBody() {
    return m_world ? m_world->getRigidBody(m_entity) : nullptr;
}

World::World(ComponentTypeID type_id, Scene* scene, vec3 gravity)
  : ComponentContainerBase{type_id}, m_scene{scene} {
    m_broadphase       = std::make_unique<btDbvtBroadphase>();
    m_collision_config = std::make_unique<btDefaultCollisionConfiguration>();
    m_dispatcher       = std::make_unique<btCollisionDispatcher>(m_collision_config.get());
    m_solver           = std::make_unique<btSequentialImpulseConstraintSolver>();

    m_world = std::make_unique<btDiscreteDynamicsWorld>(
        m_dispatcher.get(), m_broadphase.get(), m_solver.get(), m_collision_config.get()
    );
    m_world->setGravity(btVector3(gravity.x, gravity.y, gravity.z));
}

void World::update(double time_since_last_update) {
    m_world->stepSimulation(1.f / 60.f, 10);
}

PhysicsComponent World::createComponent(EntityID entity_id) {
    return {this, entity_id};
}

bool World::createComponent(EntityID entity_id, xml::Element const* properties) {
    if(xml::Element const* rb = properties->FirstChildElement("RigidBody")) {
        std::string     shape_name;
        units::Kilogram mass;
        vec3            center_of_mass_offset = {0, 0, 0};
        if(!xml::getAttribute(rb, "shape", shape_name)) return false;
        if(!xml::getAttribute(rb, "mass", mass.value)) return false;
        xml::getAttribute(rb, "center_of_mass_offset", center_of_mass_offset);

        Asset<CollisionShape> collision_shape{shape_name};

        if(!collision_shape) return false;

        return createRigidBody(entity_id, collision_shape, mass, center_of_mass_offset) != nullptr;
    }
    return true;
}

bool World::deleteComponent(EntityID entity_id) {
    auto it = m_rigid_bodys.find(entity_id);
    if(it != m_rigid_bodys.end()) {
        m_rigid_bodys.erase(it);
    }

    return true;
}

PhysicsComponent World::getComponent(EntityID entity_id) {
    return {this, entity_id};
}

RigidBody* World::createRigidBody(
    In<EntityID>              entity,
    In<Asset<CollisionShape>> collision_shape,
    In<units::Kilogram>       mass,
    In<vec3>                  center_of_mass_offset
) {
    if(!collision_shape) return nullptr;

    auto rigid_body =
        std::make_unique<RigidBody>(this, entity, collision_shape, center_of_mass_offset, mass);

    RigidBody* rigid_body_ptr = rigid_body.get();

    m_rigid_bodys.emplace(entity, std::move(rigid_body));

    return rigid_body_ptr;
}

RigidBody* World::getRigidBody(In<EntityID> entity) {
    auto it = m_rigid_bodys.find(entity);
    if(it != m_rigid_bodys.end()) { return it->second.get(); }

    return nullptr;
}

EntityID World::rayTestFirst(In<vec3> ray_start, In<vec3> ray_end) {
    btVector3 start = {ray_start.x, ray_start.y, ray_start.z};
    btVector3 end   = {ray_end.x, ray_end.y, ray_end.z};

    btCollisionWorld::ClosestRayResultCallback ray_Callback(start, end);
    // ray_Callback.m_collisionFilterMask = 0;
    m_world->rayTest(start, end, ray_Callback);

    if(!ray_Callback.hasHit()) return EntityID::INVALID;

    RigidBody* rb = (RigidBody*)ray_Callback.m_collisionObject->getUserPointer();

    return rb ? rb->getEntityID() : EntityID::INVALID;

    return EntityID::INVALID;
}

} // namespace bembel::physics
