module;
#include <memory>
#include <string_view>
module bembel.examples.chess;

import bembel;

namespace bembel::examples::chess {
using namespace base;
using namespace kernel;
using namespace graphics;
using namespace gui;

Application::Application() {
    RenderingPipeline::Stage::registerStageType<SelectionRenderingStage>("SelectionRenderingStage");
}

Application::~Application() {}

bool Application::init(std::span<std::string_view> args) {
    logInfo("Loading Application Settings");
    /** @ToDo C++26 use #embed
     *  static constexpr char config[] = {
     *      #embed "../config.xml"
     *  };
     *  if(!m_engine.parseSetting(config)) return false;
     */
    if(!m_engine.loadSetting("chess/config.xml")) return false;

    kernel::i18n::Localisation::init(args, "local");

    auto pipline = getSystem<GraphicSystem>()->getRenderingPipelines()[0].get();

    m_camera = std::make_unique<CameraControle>(pipline->getCamera());

    m_scene = std::make_shared<Scene>();
    m_scene->registerComponentType<DirectionalLight>();
    m_scene->registerComponentType<Geometry>();
    m_scene->loadAssets("scenes/assets.xml");

    pipline->setScene(m_scene);
    getSystem<PhysicsSystem>()->addScene(m_scene);
    getSystem<ParticleSystem>()->addScene(m_scene);

    m_chess_board = std::make_unique<ChessBoard>(m_scene.get());

    Entity ligth = {*m_scene, m_scene->createEntity()};
    ligth.assign<DirectionalLight>(vec3(5.0f), glm::normalize(vec3(-0.3, -1, -0.2)), true);

    logInfo("Initalizing Game");
    m_game_logic = runGameLogic(
        m_chess_board.get(),
        pipline->getCamera().get(),
        getSystem<GuiSystem>()->getGUI("main")->getWidget<LabelWidget>("Label"),
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
    auto pipline = getSystem<GraphicSystem>()->getRenderingPipelines()[0].get();

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
