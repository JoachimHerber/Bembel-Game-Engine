module;
#include <glm/glm.hpp>
#include <memory>
#include <string_view>
module bembel.examples.chess;

import bembel;

namespace bembel::examples::chess {
using namespace base;
using namespace kernel;
using namespace graphics;
using namespace gui;

Application::Application() : kernel::Application() {
    m_graphic_system = m_engine.addSystem<GraphicSystem>();
    m_physics_system = m_engine.addSystem<PhysicsSystem>();
    m_gui_system     = m_engine.addSystem<GuiSystem>();

    RenderingPipeline::Stage::registerStageType<SelectionRenderingStage>("SelectionRenderingStage");

    events::addHandler<WindowShouldCloseEvent>(this);
    events::addHandler<FrameBufferResizeEvent>(this);
    events::addHandler<KeyPressEvent>(this);
}

Application::~Application() {
    events::removeHandler<WindowShouldCloseEvent>(this);
    events::removeHandler<FrameBufferResizeEvent>(this);
    events::removeHandler<KeyPressEvent>(this);
}

bool Application::init(std::span<std::string_view> args) {
    logInfo("Loading Application Settings");
    if(!m_engine.loadSetting("chess/config.xml")) return false;

    kernel::i18n::Localisation::init(args, "local");

    auto pipline = m_graphic_system->getRenderingPipelines()[0].get();

    m_camera = std::make_unique<CameraControle>(pipline->getCamera());

    m_scene = std::make_shared<Scene>();
    m_scene->registerComponentType<DirectionalLight>();
    m_scene->loadAssets("scenes/assets.xml");

    pipline->setScene(m_scene);
    m_physics_system->addScene(m_scene);

    m_chess_board = std::make_unique<ChessBoard>(m_scene.get());

    Entity<> ligth = {*m_scene, m_scene->createEntity()};
    ligth.createComponent<DirectionalLight>(
        vec3(5.0f), glm::normalize(glm::vec3(-0.3, -1, -0.2)) //, std::make_unique<ShadowMap>(4096)
    );

    logInfo("Initalizing Game");
    m_game_logic = runGameLogic(
        m_chess_board.get(),
        pipline->getCamera().get(),
        m_gui_system->getGUI("main")->getWidget<LabelWidget>("Label"),
        m_engine.input.mouse.getButton(0)->press_signal,
        m_frame_sync
    );

    logInfo("Initalizing Camera");
    m_camera->setCameraOffset(vec3(8, 0.5f, 8));
    m_camera->enableManualControle(true);

    logInfo("Initalizing Systems");
    m_engine.initSystems();
    return true;
}

void Application::cleanup() {
    m_chess_board.reset();
    m_scene.reset();

    m_engine.shutdownSystems();
    // m_engine.assets.deleteUnusedAssets();
    m_engine.display.closeOpenWindows();
}

void Application::update(double time) {
    m_camera->update(time);
    m_frame_sync();
}

void Application::handleEvent(In<WindowShouldCloseEvent> event) {
    quit();
}

void Application::handleEvent(In<FrameBufferResizeEvent> event) {
    auto pipline = m_graphic_system->getRenderingPipelines()[0].get();

    pipline->setResulution(event.size);
    pipline->getCamera()->setUpProjection(
        60.0f * 3.14159265359f / 180.0f, event.size.x / event.size.y, 0.1f, 1000.0f
    );
}

void Application::handleEvent(In<KeyPressEvent> event) {
    if((event.mods & 0x2) && event.key_id == 'D') { // [Ctrl] + [D]
        events::broadcast(ConfigurePhysicsDebugRenderStageEvent{
            .enable     = ConfigurePhysicsDebugRenderStageEvent::TOGGLE,
            .depth_test = ConfigurePhysicsDebugRenderStageEvent::TRUE,
        });
    }
}

} // namespace bembel::examples::chess
