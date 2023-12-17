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
    if(!collision_shape) throw Exeption("Invalid collision shape");

    btVector3 local_inertia(0, 0, 0);
    if(mass.value != 0)
        collision_shape->getCollisionShape()->calculateLocalInertia(mass.value, local_inertia);
    return local_inertia;
}

btRigidBody::btRigidBodyConstructionInfo createRigidBodyConstructionInfo(
    In<Asset<CollisionShape>> collision_shape,
    In<units::Kilogram>       mass,
    MotionState*              motion_state,
    In<float>                 friction
) {
    btRigidBody::btRigidBodyConstructionInfo info{
        btScalar(mass.value),
        motion_state,
        collision_shape->getCollisionShape(),
        calculateLocalInertia(collision_shape, mass)};

    info.m_friction = friction;
    return info;
}

RigidBody::RigidBody(
    In<World*>                world,
    In<EntityID>              entity,
    In<Asset<CollisionShape>> collision_shape,
    In<vec3>                  center_of_mass_offset,
    In<units::Kilogram>       mass,
    In<float>                 friction
)
  : m_world{world}
  , m_entity{entity}
  , m_motion_state{world->getScene()->getComponent<Transform>(entity)}
  , m_collision_shape{collision_shape}
  , m_rigid_body{
        createRigidBodyConstructionInfo(collision_shape, mass, &m_motion_state, friction)} {
    m_motion_state.setCenterOfMassOffset(
        {center_of_mass_offset.x, center_of_mass_offset.y, center_of_mass_offset.z}
    );
    m_rigid_body.setUserPointer(this);
    m_world->getWorld()->addRigidBody(&m_rigid_body);
}

RigidBody::~RigidBody() {
    m_world->getWorld()->removeRigidBody(&m_rigid_body);
}

bool RigidBody::makeKinematic() {
    m_rigid_body.setCollisionFlags(btCollisionObject::CF_KINEMATIC_OBJECT);
    m_rigid_body.setActivationState(DISABLE_DEACTIVATION);
    return true;
}

bool RigidBody::makeStatic() {
    m_world->getWorld()->removeRigidBody(&m_rigid_body);

    m_rigid_body.setCollisionFlags(btCollisionObject::CF_STATIC_OBJECT);
    m_rigid_body.setActivationState(ISLAND_SLEEPING);
    m_rigid_body.setLinearFactor(btVector3(0, 0, 0));
    m_rigid_body.setAngularFactor(btVector3(0, 0, 0));

    m_world->getWorld()->addRigidBody(&m_rigid_body);
    return true;
}

bool RigidBody::makeDynamic(In<units::Kilogram> mass) {
    btVector3 local_inertia = calculateLocalInertia(m_collision_shape, mass);

    m_world->getWorld()->removeRigidBody(&m_rigid_body);

    m_rigid_body.setMassProps(mass.value, local_inertia);
    m_rigid_body.setCollisionFlags(btCollisionObject::CF_DYNAMIC_OBJECT);
    m_rigid_body.setActivationState(ISLAND_SLEEPING);
    m_rigid_body.updateInertiaTensor();
    m_rigid_body.clearForces();

    m_world->getWorld()->addRigidBody(&m_rigid_body);
    return true;
}

vec3 RigidBody::getLinearVelocity() {
    btVector3 v = m_rigid_body.getLinearVelocity();
    return {v.getX(), v.getY(), v.getZ()};
}

vec3 RigidBody::getAngularVelocity() {
    btVector3 v = m_rigid_body.getAngularVelocity();
    return {v.getX(), v.getY(), v.getZ()};
}

RigidBodyContainer::RigidBodyContainer(ComponentTypeID type_id, Scene* scene)
  : ComponentContainerBase{type_id}, m_scene{scene} {}

bool RigidBodyContainer::assignComponent(
    EntityID                  entity_id,
    In<Asset<CollisionShape>> collision_shape,
    In<vec3>                  center_of_mass_offset,
    In<units::Kilogram>       mass,
    In<float>                 friction
) {
    if(!collision_shape) {
        logError("Can't create RigidBody without a valid collision shape");
        return false;
    }

    auto it = m_rigid_bodys.find(entity_id);
    if(it != m_rigid_bodys.end()) {
        logWarning("Entity already has RigidBody");
        return false;
    }

    m_rigid_bodys.emplace(
        entity_id,
        std::make_unique<RigidBody>(
            m_scene->getDataContainer<World>(),
            entity_id,
            collision_shape,
            center_of_mass_offset,
            mass,
            friction
        )
    );
    return true;
}

bool RigidBodyContainer::deserializeComponent(EntityID entity_id, xml::Element const* properties) {
    std::string     shape_name;
    units::Kilogram mass;
    vec3            center_of_mass_offset = {0, 0, 0};
    float           friction              = 0.5f;
    if(!xml::getAttribute(properties, "shape", shape_name)) return false;
    if(!xml::getAttribute(properties, "mass", mass.value)) return false;
    xml::getAttribute(properties, "center_of_mass_offset", center_of_mass_offset);
    xml::getAttribute(properties, "friction", friction);

    Asset<CollisionShape> shape{shape_name};

    return assignComponent(entity_id, shape, center_of_mass_offset, mass, friction);
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
