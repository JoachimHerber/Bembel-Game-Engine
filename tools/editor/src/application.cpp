module;
#include <filesystem>
#include <memory>
#include <string_view>
module bembel.tools.editor;

namespace bembel::tools {
using namespace bembel::base;
using namespace bembel::kernel;
using namespace bembel::graphics;
using namespace bembel::gui;
using namespace bembel::physics;

Application::Application() : kernel::Application() {
    m_graphic_system = m_engine.addSystem<GraphicSystem>();
    m_physics_system = m_engine.addSystem<PhysicsSystem>();
    m_gui_system     = m_engine.addSystem<GuiSystem>();

    events::addHandler<WindowShouldCloseEvent>(this);
    events::addHandler<FrameBufferResizeEvent>(this);
}

Application::~Application() {
    events::removeHandler<WindowShouldCloseEvent>(this);
    events::removeHandler<FrameBufferResizeEvent>(this);
}

bool Application::init() {
    auto& asset_locator = AssetLocator::getInstance();
    asset_locator.addGenericAssetDirectory("../assets");
    asset_locator.addGenericAssetDirectory("../assets/gui");
    asset_locator.addGenericAssetDirectory("../assets/shader");
    asset_locator.addGenericAssetDirectory("../assets/fonts");

    m_main_window                   = m_engine.display.createWindow();
    m_main_window->background_color = vec3{0.9f, 0.9f, 0.9f};
    m_main_window->setDisplayMode(std::make_shared<WindowDisplayMode>(uvec2{1200, 900}));

    auto& viewport = m_main_window->createViewport(vec2{0.00f, 0.f}, vec2{1.f, 1.f});

    m_main_window->open("Bembel Editor");

    initUserInterface();
    viewport.addView(&m_gui->view);


    m_mgr = std::make_unique<ModelManager>(m_tabs->addTab(u8"Models"));

    // m_tabs->addTab(u8"Models");
    m_tabs->addTab(u8"Tab2");
    m_tabs->addTab(u8"Tab3");

    m_engine.initSystems();

    return true;
}

void Application::cleanup() {
    m_engine.shutdownSystems();
    m_engine.display.closeOpenWindows();
}

void Application::update(double time) {}

void Application::handleEvent(In<WindowShouldCloseEvent> event) {
    quit();
}

void Application::handleEvent(In<FrameBufferResizeEvent> event) {
    m_tabs->size = event.size;
}

bool Application::initUserInterface() {
    Asset<ShaderProgram> gui_shader;
    Asset<gui::Style>    gui_style;

    gui_shader.request("gui.shader-program");
    gui_style.request("dark_gui_style.xml");

    if(!gui_shader || !gui_style) return false;

    m_gui = m_gui_system->createGUI("main");
    m_gui->renderer.init(gui_shader, gui_style);

    auto& root = m_gui->root_widget;

    m_tabs      = root.createChildWidget<TabGroupWidget>();
    auto layout = root.setLayout<RelativeWidgetLayout>();
    layout->addWidget(m_tabs, {0.0f, 0.0f}, {1.0f, 1.0f});

    m_tabs->size     = m_main_window->getFrameBufferSize();
    m_tabs->position = ivec2{0, 0};

    return true;
}

} // namespace bembel::tools
