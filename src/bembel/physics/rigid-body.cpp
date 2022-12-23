module;
#include <bullet/btBulletDynamicsCommon.h>

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

RigidBody RigidBody::Container::createComponent(EntityID entity_id) {
    if(std::to_underlying(entity_id) >= m_data.size())
        m_data.resize(std::to_underlying(entity_id) + 1);
    return {this, &(m_data[std::to_underlying(entity_id)])};
}

bool RigidBody::Container::createComponent(EntityID entity_id, xml::Element const* properties) {
    std::string shape_name;
    double      mass;
    if(!xml::getAttribute(properties, "shape", shape_name)) return false;
    if(!xml::getAttribute(properties, "mass", mass)) return false;

    AssetHandle collision_shape =
        m_scene->getAssetManager().getAssetHandle<CollisionShape>(shape_name);

    if(!m_scene->getAssetManager().isHandelValid(collision_shape)) return false;

    return createRigidBody(entity_id, collision_shape, mass);
}

RigidBody RigidBody::Container::createComponent(
    EntityID entity_id, AssetHandle collision_shape, units::Kilogram mass
) {
    if(createRigidBody(entity_id, collision_shape, mass.value)) {
        return {this, &m_data[std::to_underlying(entity_id)]};
    }
    return {this, nullptr};
}

bool RigidBody::Container::deleteComponent(EntityID entity_id) {
    if(std::to_underlying(entity_id) > m_data.size()) {
        auto& component = m_data[std::to_underlying(entity_id)];
        if(component.rigid_body) m_world->removeCollisionObject(component.rigid_body.get());

        m_scene->getAssetManager().decrementAssetRefCount(component.collision_shape);

        component.rigid_body.reset();
        component.motion_state.reset();
    }
    return true;
}

RigidBody RigidBody::Container::getComponent(EntityID entity_id) {
    assert(std::to_underlying(entity_id) < m_data.size());
    return {this, &m_data[std::to_underlying(entity_id)]};
}

bool RigidBody::Container::createRigidBody(
    EntityID entity_id, AssetHandle collision_shape, btScalar mass
) {
    auto transform = m_scene->getComponent<Transform>(entity_id);

    auto shape = m_scene->getAssetManager().getAsset<CollisionShape>(collision_shape);
    if(!shape) return false;

    btVector3 local_inertia(0, 0, 0);
    if(mass != 0) shape->getCollisionShape()->calculateLocalInertia(mass, local_inertia);

    if(std::to_underlying(entity_id) >= m_data.size()) {
        m_data.resize(std::to_underlying(entity_id) + 1);
    }

    auto& component = m_data[std::to_underlying(entity_id)];

    component.collision_shape = collision_shape;
    component.motion_state    = std::make_unique<MotionState>(transform);

    btRigidBody::btRigidBodyConstructionInfo rbInfo(
        mass, component.motion_state.get(), shape->getCollisionShape(), local_inertia
    );

    component.rigid_body = std::make_unique<btRigidBody>(rbInfo);

    m_world->addRigidBody(component.rigid_body.get());
    m_scene->getAssetManager().incrementAssetRefCount(collision_shape);

    return true;
}

void RigidBody::setIsKinematic() {
    if(m_data && m_data->rigid_body) {
        m_data->rigid_body->setCollisionFlags(btCollisionObject::CF_KINEMATIC_OBJECT);
        m_data->rigid_body->setActivationState(DISABLE_DEACTIVATION);
    }
}

} // namespace bembel::physics
