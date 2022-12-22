module;
#include <bullet/btBulletDynamicsCommon.h>

#include <memory>
module bembel.physics;

import bembel.base;
import bembel.kernel;

namespace bembel::physics {
using namespace bembel::base;
using namespace bembel::kernel;

World::World(std::shared_ptr<Scene> scene, vec3 gravity) : m_scene{scene} {
    m_broadphase       = std::make_unique<btDbvtBroadphase>();
    m_collision_config = std::make_unique<btDefaultCollisionConfiguration>();
    m_dispatcher       = std::make_unique<btCollisionDispatcher>(m_collision_config.get());
    m_solver           = std::make_unique<btSequentialImpulseConstraintSolver>();

    m_world = std::make_unique<btDiscreteDynamicsWorld>(
        m_dispatcher.get(), m_broadphase.get(), m_solver.get(), m_collision_config.get()
    );
    m_world->setGravity(btVector3(gravity.x, gravity.y, gravity.z));

    scene->registerComponentType<Transform>();
    scene->registerComponentType<RigidBody>(m_world.get());

    m_transformations = scene->getComponentContainer<Transform>();
    m_rigid_bodies    = scene->getComponentContainer<RigidBody>();
}

void World::update(double time_since_last_update) {
    m_world->stepSimulation(1.f / 60.f, 10);

    auto& entities        = m_scene->getEntitys();
    auto& transformations = m_transformations->getComponentData();
    auto& rigid_bodys     = m_rigid_bodies->getComponentData();

    for(usize entity = 0; entity < entities.size(); ++entity) {
        if((entities[entity] & m_rigid_bodies->getComponentMask()) == 0) continue;

        btTransform trans;
        if(rigid_bodys[entity].motion_state) {
            rigid_bodys[entity].motion_state->getWorldTransform(trans);
        }
        if((entities[entity] & m_transformations->getComponentMask()) != 0) {
            auto pos                         = trans.getOrigin();
            auto rot                         = trans.getRotation();
            transformations[entity].position = vec3{pos.getX(), pos.getY(), pos.getZ()};
            transformations[entity].rotation = quat{rot.w(), rot.x(), rot.y(), rot.z()};
        }
    }
}

} // namespace bembel::physics
