export module bembel.physics:System;

import std;
import bembel.base;
import bembel.kernel;
import bembel.graphics;

import :CollisionShape;
import :World;
import :DebugRenderer;
import :RigidBody;

namespace bembel::physics {
using namespace bembel::base;
using namespace bembel::kernel;
using namespace bembel::graphics;

export class PhysicsSystem : public System {
  public:
    PhysicsSystem(In<Engine*> engine);
    PhysicsSystem(PhysicsSystem const&)            = delete;
    PhysicsSystem& operator=(PhysicsSystem const&) = delete;
    ~PhysicsSystem();

    void addScene(std::shared_ptr<Scene> scene) {
        scene->registerComponentType<Transform>();
        scene->registerComponentType<RigidBody>();
        scene->createDataContainer<World>();
        m_scenes.push_back(std::move(scene));
    }

    void handleEvent(In<AppUpdateEvent>);

    virtual bool configure(xml::Element const*) override { return true; }

    virtual bool init() override { return true; }
    virtual void shutdown() override { m_scenes.clear(); }

  private:
    EventHandlerGuard<AppUpdateEvent> m_guard = {this};

    Engine* m_engine;

    std::vector<std::shared_ptr<Scene>> m_scenes;
};

} // namespace bembel::physics
