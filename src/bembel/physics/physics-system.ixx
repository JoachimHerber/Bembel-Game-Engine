module;
#include "bembel/pch.h"
export module bembel.physics:System;

import bembel.base;
import bembel.kernel;

import :CollisionShape;
import :World;

namespace bembel::physics {
using namespace bembel::base;
using namespace bembel::kernel;

export class PhysicsSystem : public System {
  public:
    PhysicsSystem(Engine& engine) : System{"Physics"}, m_engine{engine} {
        engine.assets.registerAssetType<CollisionShape>();

        CollisionShape::initFactory();
    }
    PhysicsSystem(PhysicsSystem const&)            = delete;
    PhysicsSystem& operator=(PhysicsSystem const&) = delete;
    ~PhysicsSystem()                               = default;

    template <typename... TArgs>
    World* addScene(std::shared_ptr<Scene> scene, TArgs&&... args) {
        m_worlds.push_back(std::make_unique<World>(scene, std::forward<TArgs>(args)...));
        return m_worlds.back().get();
    }

    virtual bool configure(xml::Element const*) override { return true; }

    virtual bool init() override { return true; }
    virtual void shutdown() override { m_worlds.clear(); }
    virtual void update(double time_since_last_update) override {
        for(auto& it : m_worlds) it->update(time_since_last_update);
    }

  private:
    Engine& m_engine;

    std::vector<std::unique_ptr<World>> m_worlds;
};

} // namespace bembel::physics
