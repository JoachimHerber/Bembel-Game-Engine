module;
#include <btBulletDynamicsCommon.h>

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
}

void World::update(double time_since_last_update) {
    m_world->stepSimulation(1.f / 60.f, 10);
}

} // namespace bembel::physics
