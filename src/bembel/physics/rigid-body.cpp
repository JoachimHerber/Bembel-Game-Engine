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

RigidBody::RigidBody(In<World*> world, In<EntityID> entity)
  : m_world{world}
  , m_entity{entity}
  , m_motion_state{world->getScene()->getComponent<Transform>(entity)} {}
RigidBody::~RigidBody() {
    if(m_rigid_body) m_world->getWorld()->removeRigidBody(m_rigid_body.get());
}

bool RigidBody::init(
    In<Asset<CollisionShape>> collision_shape,
    In<vec3>                  center_of_mass_offset,
    In<units::Kilogram>       mass,
    In<float>                 friction
) {
    if(m_rigid_body) return false;

    if(!collision_shape) return false;

    m_collision_shape = collision_shape;

    m_motion_state.setCenterOfMassOffset(
        {center_of_mass_offset.x, center_of_mass_offset.y, center_of_mass_offset.z}
    );

    btRigidBody::btRigidBodyConstructionInfo info{
        btScalar(mass.value),
        &m_motion_state,
        collision_shape->getCollisionShape(),
        calculateLocalInertia(collision_shape, mass)};

    info.m_friction = friction;

    m_rigid_body = std::make_unique<btRigidBody>(info);
    m_rigid_body->setUserPointer(this);
    m_world->getWorld()->addRigidBody(m_rigid_body.get());
    return true;
}
bool RigidBody::makeKinematic() {
    if(!m_rigid_body) return false;
    m_rigid_body->setCollisionFlags(btCollisionObject::CF_KINEMATIC_OBJECT);
    m_rigid_body->setActivationState(DISABLE_DEACTIVATION);
    return true;
}
bool RigidBody::makeStatic() {
    if(!m_rigid_body) return false;
    m_world->getWorld()->removeRigidBody(m_rigid_body.get());

    m_rigid_body->setCollisionFlags(btCollisionObject::CF_STATIC_OBJECT);
    m_rigid_body->setActivationState(ISLAND_SLEEPING);
    m_rigid_body->setLinearFactor(btVector3(0, 0, 0));
    m_rigid_body->setAngularFactor(btVector3(0, 0, 0));

    m_world->getWorld()->addRigidBody(m_rigid_body.get());
    return true;
}

bool RigidBody::makeDynamic(In<units::Kilogram> mass) {
    if(!m_rigid_body || !m_collision_shape) return false;
    btVector3 local_inertia = calculateLocalInertia(m_collision_shape, mass);

    m_world->getWorld()->removeRigidBody(m_rigid_body.get());

    m_rigid_body->setMassProps(mass.value, local_inertia);
    m_rigid_body->setCollisionFlags(btCollisionObject::CF_DYNAMIC_OBJECT);
    m_rigid_body->setActivationState(ISLAND_SLEEPING);
    m_rigid_body->updateInertiaTensor();
    m_rigid_body->clearForces();

    m_world->getWorld()->addRigidBody(m_rigid_body.get());
    return true;
}

vec3 RigidBody::getLinearVelocity() {
    btVector3 v = m_rigid_body->getLinearVelocity();
    return {v.getX(), v.getY(), v.getZ()};
}

vec3 RigidBody::getAngularVelocity() {
    btVector3 v = m_rigid_body->getAngularVelocity();
    return {v.getX(), v.getY(), v.getZ()};
}

RigidBodyContainer::RigidBodyContainer(ComponentTypeID type_id, Scene* scene)
  : ComponentContainerBase{type_id}, m_scene{scene} {}

RigidBody* RigidBodyContainer::createComponent(EntityID entity_id) {
    auto it = m_rigid_bodys.find(entity_id);
    if(it != m_rigid_bodys.end()) {
        logWarning("Entity already has RigidBody");
        return it->second.get();
    }
    auto rb     = std::make_unique<RigidBody>(m_scene->getDataContainer<World>(), entity_id);
    auto rp_ptr = rb.get();
    m_rigid_bodys.emplace(entity_id, std::move(rb));

    return rp_ptr;
}

bool RigidBodyContainer::createComponent(EntityID entity_id, xml::Element const* properties) {
    auto it = m_rigid_bodys.find(entity_id);
    if(it != m_rigid_bodys.end()) {
        logError("Entity already has RigidBody");
        return false;
    }

    std::string     shape_name;
    units::Kilogram mass;
    vec3            center_of_mass_offset = {0, 0, 0};
    float           friction              = 0.5f;
    if(!xml::getAttribute(properties, "shape", shape_name)) return false;
    if(!xml::getAttribute(properties, "mass", mass.value)) return false;
    xml::getAttribute(properties, "center_of_mass_offset", center_of_mass_offset);
    xml::getAttribute(properties, "friction", friction);

    Asset<CollisionShape> collision_shape{shape_name};

    if(!collision_shape) return false;

    auto rb = std::make_unique<RigidBody>(m_scene->getDataContainer<World>(), entity_id);
    if(!rb->init(collision_shape, center_of_mass_offset, mass, friction)) { return false; }

    m_rigid_bodys.emplace(entity_id, std::move(rb));

    return true;
}

bool RigidBodyContainer::deleteComponent(EntityID entity_id) {
    auto it = m_rigid_bodys.find(entity_id);
    if(it != m_rigid_bodys.end()) { m_rigid_bodys.erase(it); }

    return true;
}

RigidBody* RigidBodyContainer::getComponent(EntityID entity_id) {
    auto it = m_rigid_bodys.find(entity_id);
    if(it != m_rigid_bodys.end()) return it->second.get();

    return nullptr;
}

} // namespace bembel::physics
