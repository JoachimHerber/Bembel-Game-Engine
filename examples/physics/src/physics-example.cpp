﻿module;
#include <filesystem>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
module bembel.examples.physics;

import bembel;

namespace bembel::examples::physics {
using namespace bembel::base;
using namespace bembel::kernel;
using namespace bembel::graphics;
using namespace bembel::physics;
using namespace bembel::physics::units::literals;
using namespace bembel::gui;

PhysicsExample::PhysicsExample() : Application() {
    m_graphic_system = m_engine.addSystem<GraphicSystem>();
    m_physics_system = m_engine.addSystem<PhysicsSystem>();
    m_gui_system     = m_engine.addSystem<GuiSystem>();

    events::addHandler<WindowShouldCloseEvent>(this);
    events::addHandler<FrameBufferResizeEvent>(this);
    events::addHandler<KeyPressEvent>(this);
}

PhysicsExample::~PhysicsExample() {
    events::removeHandler<WindowShouldCloseEvent>(this);
    events::removeHandler<FrameBufferResizeEvent>(this);
    events::removeHandler<KeyPressEvent>(this);
}

bool PhysicsExample::init(std::span<std::string_view>) {
    if(!m_engine.loadSetting("physics/config.xml")) return false;
    auto pipline = m_graphic_system->getRenderingPipelines()[0].get();

    m_camera = std::make_unique<CameraControle>(pipline->getCamera());

    m_gui = m_gui_system->getGUI("main");

    m_scene = std::make_shared<Scene>();

    auto& rendering_pipeline = m_graphic_system->getRenderingPipelines()[0];

    rendering_pipeline->setScene(m_scene);
    m_physics_system->addScene(m_scene);

    m_scene->loadAssets("scenes/assets.xml");
    m_scene->loadScene("scenes/physics-demo.scene");

    m_stirring_stick = m_scene->createEntity();

    m_scene->createComponent<Transform>(m_stirring_stick, vec3(0, -0.75, 0));
    m_scene->createComponent<Geometry>(
        m_stirring_stick, Asset<GeometryModel>("stirring_stick"), vec3(4.5f, 0.5f, 0.1f)
    );
    auto stirring_stick = m_scene->createComponent<PhysicsComponent>(m_stirring_stick);
    RigidBody* rb = stirring_stick.createRigidBody(Asset<CollisionShape>("stirring_stick"), 0.0_kg);
    rb->makeKinematic();

    m_engine.initSystems();
    return true;
}

void PhysicsExample::cleanup() {
    m_camera.reset();
    m_scene.reset();

    m_engine.shutdownSystems();
    assets::deleteUnusedAssets();
    m_engine.display.closeOpenWindows();
}

void PhysicsExample::update(double time) {
    m_rotation += time;

    auto transform = m_scene->getComponent<Transform>(m_stirring_stick);
    if(transform) transform->rotation = quat(glm::angleAxis(float(m_rotation), vec3(0, 1, 0)));
}

void PhysicsExample::handleEvent(In<WindowShouldCloseEvent> event) {
    quit();
}

void PhysicsExample::handleEvent(In<FrameBufferResizeEvent> event) {
    m_graphic_system->getRenderingPipelines()[0]->setResulution(event.size);
}

void PhysicsExample::handleEvent(In<KeyPressEvent> event) {
    if((event.mods & 0x2) && event.key_id == 'D') { // [Ctrl] + [D]
        events::broadcast(ConfigurePhysicsDebugRenderStageEvent{
            .enable     = ConfigurePhysicsDebugRenderStageEvent::TOGGLE,
            .depth_test = ConfigurePhysicsDebugRenderStageEvent::FALSE,
        });
    }
}

} // namespace bembel::examples::physics
