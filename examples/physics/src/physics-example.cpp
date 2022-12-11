module;
#include "bembel/pch.h"
module bembel.examples.physics;

import bembel;

namespace bembel::examples::physics {
using namespace bembel::base;
using namespace bembel::kernel;
using namespace bembel::graphics;
using namespace bembel::physics;
using namespace bembel::gui;

PhysicsExample::PhysicsExample() : Application() {
    m_graphic_system = m_engine.addSystem<GraphicSystem>();
    m_physics_system = m_engine.addSystem<PhysicsSystem>();
    m_gui_system     = m_engine.addSystem<GuiSystem>();

    events::addHandler<WindowShouldCloseEvent>(this);
}

PhysicsExample::~PhysicsExample() {
    events::removeHandler<WindowShouldCloseEvent>(this);
}

bool PhysicsExample::init() {
    if(!m_engine.loadSetting("physics/config.xml")) return false;
    auto pipline = m_graphic_system->getRenderingPipelines()[0].get();

    m_camera = std::make_unique<CameraControle>(pipline->getCamera());

    m_gui    = m_gui_system->getGUI("main");

    m_scene = std::make_shared<Scene>(m_engine.assets);
    m_graphic_system->getRenderingPipelines()[0]->setScene(m_scene);
    m_physics_system->addScene(m_scene);

    Entity<> ligth = {*m_scene, m_scene->createEntity()};
    ligth.createComponent<DirectionalLightComponent>(
        vec3(5.0f), glm::normalize(glm::vec3(-0.3, -1, -0.2))
    );

    m_scene->loadAssets("assets/assets.xml");
    m_scene->loadScene("scenes/physics-demo.scene");

    m_engine.initSystems();
    return true;
}

void PhysicsExample::cleanup() {
    m_camera.reset();
    m_scene.reset();

    m_engine.shutdownSystems();
    m_engine.assets.deleteUnusedAssets();
    m_engine.display.closeOpenWindows();
}

void PhysicsExample::update(double time) {
}

void PhysicsExample::handleEvent(const WindowShouldCloseEvent& event) {
    quit();
}

} // namespace bembel::examples::physics
