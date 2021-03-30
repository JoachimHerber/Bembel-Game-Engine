#include "rendering-example.hpp"

#include <bembel/graphics/rendering-pipeline/rendering-pipeline.hpp>
#include <bembel/kernel/core/kernel.hpp>
#include <bembel/kernel/display/display-manager.hpp>

namespace bembel {

RenderingExample::RenderingExample()
: kernel::Application() {
    this->graphic_system = this->kernel->addSystem<graphics::GraphicSystem>();
    this->kernel->getEventManager().addHandler<kernel::WindowShouldCloseEvent>(this);
}

RenderingExample::~RenderingExample() {
}

bool RenderingExample::init() {
    if(!this->kernel->loadSetting("rendering/config.xml")) return false;

    this->camera = std::make_unique<CameraControle>(
        this->kernel->getEventManager(),
        this->graphic_system->getRenderingPipelines()[0]->getCamera());

    this->scenes.push_back(std::make_shared<kernel::Scene>(this->kernel->getAssetManager()));
    this->graphic_system->getRenderingPipelines()[0]->setScene(this->scenes[0]);

    this->scenes[0]->loadScene("scenes/material-test.scene");

    this->kernel->initSystems();
    return true;
}

void RenderingExample::cleanup() {
    this->camera.reset();
    this->scenes.clear();
    this->kernel->shutdownSystems();
    this->kernel->getAssetManager().deleteUnusedAssets();
    this->kernel->getDisplayManager().closeOpenWindows();
}

void RenderingExample::update(double time) {
    this->camera->update(time);
}

void RenderingExample::handleEvent(const kernel::WindowShouldCloseEvent& event) {
    quit();
}

} // namespace bembel
