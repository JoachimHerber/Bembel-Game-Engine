module bembel.physics;

import std;
import bembel.base;
import bembel.kernel;

namespace bembel::physics {
using namespace bembel::base;
using namespace bembel::kernel;

PhysicsSystem::PhysicsSystem(In<Engine*> engine) //
  : System{"Physics"}                            //
  , m_engine{engine}                             //
{
    RenderingPipeline::Stage::registerStageType<PhysicsDebugRenderStage>("PhysicsDebugRenderStage");

    assets::registerAssetType<CollisionShape>();

    CollisionShape::initFactory();
}

PhysicsSystem::~PhysicsSystem() {}

void PhysicsSystem::handleEvent(In<AppUpdateEvent> event) {
    for(auto& scene : m_scenes){
        scene->getDataContainer<World>()->update(event.ΔT);
    }
}

} // namespace bembel::physics
