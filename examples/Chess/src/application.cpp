﻿module;
#include "bembel/pch.h"
module bembel.examples.chess;

import bembel;

namespace bembel::examples::chess {
using namespace base;
using namespace kernel;
using namespace graphics;
using namespace gui;

Application::Application() : kernel::Application() {
    m_graphic_system = m_engine.addSystem<GraphicSystem>();
    m_gui_system     = m_engine.addSystem<GuiSystem>();

    RenderingPipeline::Stage::registerStageType<SelectionRenderingStage>("SelectionRenderingStage");

    auto& event_mgr = m_engine.getEventManager();
    event_mgr.addHandler<WindowShouldCloseEvent>(this);
    event_mgr.addHandler<FrameBufferResizeEvent>(this);
}

Application::~Application() {}

bool Application::init() {
    log().info("Loading Application Settings");
    if(!m_engine.loadSetting("chess/config.xml")) return false;
    auto pipline = m_graphic_system->getRenderingPipelines()[0].get();

    m_camera = std::make_unique<CameraControle>(m_engine.getEventManager(), pipline->getCamera());

    m_scene = std::make_shared<Scene>(m_engine.getAssetManager());
    m_scene->registerComponentType<DirectionalLightSource>();
    m_scene->loadAssets("assets/assets.xml");

    m_chess_board = std::make_unique<ChessBoard>(m_scene.get());

    Entity<> ligth = {*m_scene, m_scene->createEntity()};
    ligth.createComponent<DirectionalLightSource>(
        vec3(5.0f), glm::normalize(glm::vec3(-0.3, -1, -0.2))
    );

    m_selection_ptr = std::make_unique<SelectionPointer>(m_engine.getEventManager(), pipline);

    log().info("Initalizing Game");
    m_game_logic = std::make_unique<GameLogic>(
        m_chess_board.get(), m_selection_ptr.get(), m_camera.get(), m_gui_system->getGUI("main")
    );

    log().info("Initalizing Camera");
    pipline->setScene(m_scene);
    m_camera->setCameraOffset(vec3(8, 0.5f, 8));
    m_camera->enableManualControle(true);

    log().info("Initalizing Systems");
    m_engine.initSystems();
    return true;
}

void Application::cleanup() {
    m_chess_board.reset();
    m_scene.reset();

    m_engine.shutdownSystems();
    m_engine.getAssetManager().deleteUnusedAssets();
    m_engine.getDisplayManager().closeOpenWindows();
}

void Application::update(double time) {
    m_camera->update(time);
    m_game_logic->update(time);
}

void Application::handleEvent(WindowShouldCloseEvent const& event) {
    quit();
}

void Application::handleEvent(FrameBufferResizeEvent const& event) {
    auto pipline = m_graphic_system->getRenderingPipelines()[0].get();

    pipline->setResulution(event.size);
    pipline->getCamera()->setUpProjection(
        60.0f * 3.14159265359f / 180.0f, event.size.x / event.size.y, 0.1f, 1000.0f
    );
}

} // namespace bembel::examples::chess
