module;
#include <btBulletDynamicsCommon.h>

#include <map>
#include <memory>
export module bembel.physics:World;

import bembel.base;
import bembel.kernel;

namespace bembel::physics {
using namespace bembel::base;
using namespace bembel::kernel;

export class World : public SceneDataContainerBase {
  public:
    World(Scene* scene);
    ~World() {}

    void update(double time_since_last_update);

    Scene*                   getScene() { return m_scene; }
    btDiscreteDynamicsWorld* getWorld() { return m_world.get(); }

    EntityID rayTestFirst(In<vec3> ray_start, In<vec3> ray_end);

  private:
    Scene* m_scene;

    std::unique_ptr<btBroadphaseInterface>               m_broadphase;
    std::unique_ptr<btDefaultCollisionConfiguration>     m_collision_config;
    std::unique_ptr<btCollisionDispatcher>               m_dispatcher;
    std::unique_ptr<btSequentialImpulseConstraintSolver> m_solver;
    std::unique_ptr<btDiscreteDynamicsWorld>             m_world;
};

} // namespace bembel::physics