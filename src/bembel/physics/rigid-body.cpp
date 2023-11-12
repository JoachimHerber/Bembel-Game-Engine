module;
#include <btBulletDynamicsCommon.h>

#include <cassert>
#include <memory>
module bembel.physics;

import bembel.base;
import bembel.kernel;
import :RigidBody;
import :CollisionShape;

namespace bembel::physics {
using namespace bembel::base;
using namespace bembel::kernel;
using namespace units::literals;

btVector3 calculateLocalInertia(
    In<Asset<CollisionShape>> collision_shape, In<units::Kilogram> mass
) {
    btVector3 local_inertia(0, 0, 0);
    if(mass.value != 0)
        collision_shape->getCollisionShape()->calculateLocalInertia(mass.value, local_inertia);
    return local_inertia;
}

RigidBody ::RigidBody(
    In<World*>                world,
    In<EntityID>              entity,
    In<Asset<CollisionShape>> collision_shape,
    In<vec3>                  center_of_mass_offset,
    In<units::Kilogram>       mass
)
  : m_world{world}
  , m_entity{entity}
  , m_collision_shape{collision_shape}
  , m_motion_state{world->getScene()->getComponent<Transform>(entity), center_of_mass_offset}
  , m_rigid_body{btRigidBody::btRigidBodyConstructionInfo{
        btScalar(mass.value),
        &m_motion_state,
        collision_shape->getCollisionShape(),
        calculateLocalInertia(collision_shape, mass)}} {
    m_rigid_body.setUserPointer(this);
    m_world->getWorld()->addRigidBody(&m_rigid_body);
}
RigidBody ::~RigidBody() {
    m_world->getWorld()->removeRigidBody(&m_rigid_body);
}

void RigidBody::makeKinematic() {
    m_rigid_body.setCollisionFlags(btCollisionObject::CF_KINEMATIC_OBJECT);
    m_rigid_body.setActivationState(DISABLE_DEACTIVATION);
}
void RigidBody::makeStatic() {
    m_world->getWorld()->removeRigidBody(&m_rigid_body);

    m_rigid_body.setCollisionFlags(btCollisionObject::CF_STATIC_OBJECT);
    m_rigid_body.setActivationState(ISLAND_SLEEPING);
    m_rigid_body.setLinearFactor(btVector3(0, 0, 0));
    m_rigid_body.setAngularFactor(btVector3(0, 0, 0));

    m_world->getWorld()->addRigidBody(&m_rigid_body);
}
void RigidBody::makeDynamic(In<units::Kilogram> mass) {
    btVector3 local_inertia = calculateLocalInertia(m_collision_shape, mass);

    m_world->getWorld()->removeRigidBody(&m_rigid_body);

    m_rigid_body.setMassProps(mass.value, local_inertia);
    m_rigid_body.setCollisionFlags(btCollisionObject::CF_DYNAMIC_OBJECT);
    m_rigid_body.setActivationState(ISLAND_SLEEPING);
    m_rigid_body.updateInertiaTensor();
    m_rigid_body.clearForces();

    m_world->getWorld()->addRigidBody(&m_rigid_body);
}

vec3 RigidBody::getLinearVelocity() {
    btVector3 v = m_rigid_body.getLinearVelocity();
    return {v.getX(), v.getY(), v.getZ()};
}

vec3 RigidBody::getAngularVelocity() {
    btVector3 v = m_rigid_body.getAngularVelocity();
    return {v.getX(), v.getY(), v.getZ()};
}

} // namespace bembel::physics
