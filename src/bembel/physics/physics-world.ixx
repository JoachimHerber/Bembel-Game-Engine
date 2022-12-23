module;
#include <bullet/btBulletDynamicsCommon.h>

#include <memory>
export module bembel.physics:World;

import bembel.base;
import bembel.kernel;

import :RigidBody;

namespace bembel::physics {
using namespace bembel::base;
using namespace bembel::kernel;

export class World {
  public:
    World(std::shared_ptr<Scene> scene, vec3 gravity = {0, -9.8, 0});

    void update(double time_since_last_update);

  private:
    std::shared_ptr<Scene> m_scene;

    std::unique_ptr<btBroadphaseInterface>               m_broadphase;
    std::unique_ptr<btDefaultCollisionConfiguration>     m_collision_config;
    std::unique_ptr<btCollisionDispatcher>               m_dispatcher;
    std::unique_ptr<btSequentialImpulseConstraintSolver> m_solver;
    std::unique_ptr<btDiscreteDynamicsWorld>             m_world;
};

} // namespace bembel::physics