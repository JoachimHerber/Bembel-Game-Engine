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

World::World(Scene* scene) : m_scene{scene} {
    m_broadphase       = std::make_unique<btDbvtBroadphase>();
    m_collision_config = std::make_unique<btDefaultCollisionConfiguration>();
    m_dispatcher       = std::make_unique<btCollisionDispatcher>(m_collision_config.get());
    m_solver           = std::make_unique<btSequentialImpulseConstraintSolver>();

    m_world = std::make_unique<btDiscreteDynamicsWorld>(
        m_dispatcher.get(), m_broadphase.get(), m_solver.get(), m_collision_config.get()
    );
}

void World::update(double time_since_last_update) {
    m_world->stepSimulation(1.f / 60.f, 10);
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
