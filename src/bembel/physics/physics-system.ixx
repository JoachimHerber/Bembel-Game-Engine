module;
#include <memory>
#include <string_view>
#include <vector>
export module bembel.physics:System;

import bembel.base;
import bembel.kernel;
import bembel.graphics;

import :CollisionShape;
import :World;
import :DebugRenderer;

namespace bembel::physics {
using namespace bembel::base;
using namespace bembel::kernel;
using namespace bembel::graphics;

export class PhysicsSystem : public System {
  public:
    PhysicsSystem(Engine& engine) : System{"Physics"}, m_engine{engine} {
        RenderingPipeline::Stage::registerStageType<PhysicsDebugRenderStage>("PhysicsDebugRenderStage");

        assets::registerAssetType<CollisionShape>();

        CollisionShape::initFactory();
    }
    PhysicsSystem(PhysicsSystem const&)            = delete;
    PhysicsSystem& operator=(PhysicsSystem const&) = delete;
    ~PhysicsSystem()                               = default;

    void addScene(std::shared_ptr<Scene> scene) {
        scene->registerComponentType<Transform>();
        scene->registerComponentType<PhysicsComponent>();    
        m_scenes.push_back(std::move(scene));
    }

    virtual bool configure(xml::Element const*) override { return true; }

    virtual bool init() override { return true; }
    virtual void shutdown() override { m_scenes.clear(); }
    virtual void update(double time_since_last_update) override;

  private:
    Engine& m_engine;

    std::vector<std::shared_ptr<Scene>> m_scenes;
};

} // namespace bembel::physics
