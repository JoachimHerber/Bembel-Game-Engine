﻿module;
#include <format>
#include <memory>
#include <string>
module bembel.examples.rendering;

namespace bembel {
using namespace graphics;
using namespace bembel::base;
using namespace bembel::kernel;
using namespace bembel::gui;

RenderingExample::RenderingExample() : kernel::Application() {
    m_graphic_system = m_engine.addSystem<GraphicSystem>();
    m_gui_system     = m_engine.addSystem<GuiSystem>();
    events::addHandler<WindowShouldCloseEvent>(this);
    events::addHandler<FrameBufferResizeEvent>(this);
}

RenderingExample::~RenderingExample() {
    events::removeHandler<WindowShouldCloseEvent>(this);
    events::removeHandler<FrameBufferResizeEvent>(this);
}

bool RenderingExample::init() {
    if(!m_engine.loadSetting("rendering/config.xml")) return false;

    m_camera =
        std::make_unique<CameraControle>(m_graphic_system->getRenderingPipelines()[0]->getCamera());

    m_scenes.push_back(std::make_shared<kernel::Scene>(m_engine.assets));
    m_graphic_system->getRenderingPipelines()[0]->setScene(m_scenes[0]);

    m_scenes[0]->loadScene("scenes/material-test.scene");

    auto gui = m_gui_system->getGUI("main");

    m_label = gui->getWidget<LabelWidget>("Label");
    m_label->setHasOutline(true);

    m_engine.initSystems();
    return true;
}

void RenderingExample::cleanup() {
    m_camera.reset();
    m_scenes.clear();
    m_engine.shutdownSystems();
    m_engine.assets.deleteUnusedAssets();
    m_engine.display.closeOpenWindows();
}

void RenderingExample::update(double time) {
    m_camera->update(time);

    bembel::base::String text = std::format(
        "Cam: pos=({:.3}; {:.3}; {:.3}) pitch={:.3} yaw={:.3}",
        m_camera->getPosition().x,
        m_camera->getPosition().y,
        m_camera->getPosition().z,
        m_camera->getPitch(),
        m_camera->getYaw()
    );

    m_label->setText(text.data);
}

void RenderingExample::handleEvent(In<WindowShouldCloseEvent> event) {
    quit();
}

void RenderingExample::handleEvent(In<FrameBufferResizeEvent> event) {}

} // namespace bembel
