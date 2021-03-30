#include "chess-application.h"

#include <GLFW/glfw3.h>

#include <chrono>
#include <iostream>
#include <random>

#include "selection-rendering-stage.h"

using namespace bembel;

ChessApplication::ChessApplication()
: kernel::Application() {
    this->graphic_system = this->kernel->addSystem<graphics::GraphicSystem>();

    this->graphic_system->getRendertingStageFactory()
        .registerDefaultObjectGenerator<SelectionRenderingStage>("SelectionRenderingStage");

    auto& event_mgr = this->kernel->getEventManager();
    event_mgr.addHandler<kernel::WindowShouldCloseEvent>(this);
    event_mgr.addHandler<kernel::FrameBufferResizeEvent>(this);
}

ChessApplication::~ChessApplication() {
}

bool ChessApplication::init() {
    BEMBEL_LOG_INFO() << "Loading Application Settings";
    if(!this->kernel->loadSetting("chess/config.xml")) return false;
    auto pipline = this->graphic_system->getRenderingPipelines()[0].get();

    this->camera =
        std::make_shared<CameraControle>(this->kernel->getEventManager(), pipline->getCamera());

    BEMBEL_LOG_INFO() << "Initalizing Game";
    this->chess_game = std::make_unique<ChessGame>(
        this->kernel->getAssetManager(), this->kernel->getEventManager(), *(this->graphic_system));
    this->chess_game->resetChessBoard();
    this->chess_game->resetChessBoard();

    BEMBEL_LOG_INFO() << "Initalizing Camera";
    pipline->setScene(this->chess_game->getScene());
    this->camera->setCameraOffset(glm::vec3(8, 0.5f, 8));
    this->camera->enableManualControle(true);

    BEMBEL_LOG_INFO() << "Initalizing Systems";
    this->kernel->initSystems();
    return true;
}

void ChessApplication::cleanup() {
    this->chess_game.reset();
    this->kernel->shutdownSystems();
    this->kernel->getAssetManager().deleteUnusedAssets();
    this->kernel->getDisplayManager().closeOpenWindows();
}

void ChessApplication::update(double time) {
    this->camera->update(time);
    this->chess_game->update(time);
}

void ChessApplication::handleEvent(const kernel::WindowShouldCloseEvent& event) {
    this->quit();
}

void ChessApplication::handleEvent(const kernel::FrameBufferResizeEvent& event) {
    auto pipline = this->graphic_system->getRenderingPipelines()[0].get();

    pipline->setResulution(event.size);
    pipline->getCamera()->setUpProjection(
        60.0f * 3.14159265359f / 180.0f, event.size.x / event.size.y, 0.1f, 1000.0f);
}
