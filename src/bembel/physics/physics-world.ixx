module;
#include <bullet/btBulletDynamicsCommon.h>

#include "bembel/pch.h"
export module bembel.physics:World;

import bembel.base;
import bembel.kernel;

import :RigidBody;

namespace bembel::physics {
using namespace bembel::base;
using namespace bembel::kernel;

export class World {
  public:
    World(std::shared_ptr<Scene> scene, vec3 gravity = {0, -9.8, 0}) : m_scene{scene} {
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

    virtual void update(double time_since_last_update) { //
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
                auto pos = trans.getOrigin();
                auto rot = trans.getRotation();
                transformations[entity].position = vec3{pos.getX(), pos.getY(), pos.getZ()};
                transformations[entity].rotation = quat{rot.w(), rot.x(), rot.y(), rot.z()};
            }
        }
    }

  private:
    std::shared_ptr<Scene> m_scene;

    std::unique_ptr<btBroadphaseInterface>               m_broadphase;
    std::unique_ptr<btDefaultCollisionConfiguration>     m_collision_config;
    std::unique_ptr<btCollisionDispatcher>               m_dispatcher;
    std::unique_ptr<btSequentialImpulseConstraintSolver> m_solver;
    std::unique_ptr<btDiscreteDynamicsWorld>             m_world;

    RigidBody::Container* m_rigid_bodies;
    Transform::Container* m_transformations;
};

} // namespace bembel::physics