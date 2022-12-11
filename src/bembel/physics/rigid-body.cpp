module;
#include <bullet/btBulletDynamicsCommon.h>

#include "bembel/pch.h"
module bembel.physics;

import bembel.base;
import bembel.kernel;
import :RigidBody;
import :CollisionShape;

namespace bembel::physics {
using namespace bembel::base;
using namespace bembel::kernel;

RigidBodyComponent RigidBodyComponent::Container::createComponent(EntityID entity_id) {
    if(to_underlying(entity_id) >= m_data.size()) m_data.resize(to_underlying(entity_id) + 1);
    return {this, &(m_data[to_underlying(entity_id)])};
}

bool RigidBodyComponent::Container::createComponent(
    EntityID entity_id, xml::Element const* properties
) {
    std::string shape_name;
    double      mass;
    if(!xml::getAttribute(properties, "shape", shape_name)) return false;
    if(!xml::getAttribute(properties, "mass", mass)) return false;

    AssetHandle collision_shape =
        m_scene->getAssetManager().getAssetHandle<CollisionShape>(shape_name);

    if(!m_scene->getAssetManager().isHandelValid(collision_shape)) return false;

    return createRigidBody(entity_id, collision_shape, mass);
}

bool RigidBodyComponent::Container::deleteComponent(EntityID entity_id) {
    if(to_underlying(entity_id) > m_data.size()) {
        auto& component = m_data[to_underlying(entity_id)];
        if(component.rigid_body) m_world->removeCollisionObject(component.rigid_body.get());

        m_scene->getAssetManager().decrementAssetRefCount(component.collision_shape);

        component.rigid_body.reset();
        component.motion_state.reset();
    }
    return true;
}

RigidBodyComponent RigidBodyComponent::Container::getComponent(EntityID entity_id) {
    assert(to_underlying(entity_id) < m_data.size());
    return {this, &m_data[to_underlying(entity_id)]};
}

bool RigidBodyComponent::Container::createRigidBody(
    EntityID entity_id, AssetHandle collision_shape, btScalar mass
) {
    auto pos = m_scene->getComponent<PositionComponent>(entity_id);
    auto rot = m_scene->getComponent<RotationComponent>(entity_id);

    auto shape = m_scene->getAssetManager().getAsset<CollisionShape>(collision_shape);
    if(!shape) return false;

    btTransform transform;
    transform.setIdentity();
    if(pos) transform.setOrigin(btVector3(pos->x, pos->y, pos->z));

    btVector3 local_inertia(0, 0, 0);
    if(mass != 0) shape->getCollisionShape()->calculateLocalInertia(mass, local_inertia);

    if(to_underlying(entity_id) >= m_data.size()) { //
        m_data.resize(to_underlying(entity_id) + 1);
    }

    auto& component = m_data[to_underlying(entity_id)];

    component.collision_shape = collision_shape;
    component.motion_state    = std::make_unique<btDefaultMotionState>(transform);

    btRigidBody::btRigidBodyConstructionInfo rbInfo(
        mass, component.motion_state.get(), shape->getCollisionShape(), local_inertia
    );

    component.rigid_body = std::make_unique<btRigidBody>(rbInfo);

    m_world->addRigidBody(component.rigid_body.get());
    m_scene->getAssetManager().incrementAssetRefCount(collision_shape);

    return true;
}

} // namespace bembel::physics