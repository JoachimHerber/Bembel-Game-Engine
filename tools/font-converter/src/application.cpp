#include "./application.hpp"

#include <GLFW/glfw3.h>

Application::Application()
: bembel::Application()
, converter(std::make_unique<FontConverter>()) {
    this->gui_system = this->kernel->addSystem<bembel::GUISystem>();

    auto eventMgr = this->kernel->getEventManager();
    eventMgr->addHandler<bembel::WindowShouldCloseEvent>(this);
    eventMgr->addHandler<bembel::WindowResizeEvent>(this);
    eventMgr->addHandler<bembel::FileDropEvent>(this);

    auto escapeKey = this->kernel->getInputManager()->getKeyboard()->getKey(GLFW_KEY_ESCAPE);
    escapeKey->press_signal.addSlot<Application>(this, &Application::quit);
}

Application::~Application() {
    auto eventMgr = this->kernel->getEventManager();
    eventMgr->removeHandler<bembel::WindowShouldCloseEvent>(this);
    eventMgr->removeHandler<bembel::WindowResizeEvent>(this);
    eventMgr->removeHandler<bembel::FileDropEvent>(this);
}

bool Application::init() {
    if(!this->kernel->loadSetting("font-converter/config.xml")) return false;

    this->kernel->initSystems();

    auto window = this->kernel->getDisplayManager()->getWindow(0);
    if(!window || window->getViewports().size() < 1) return false;

    auto main_viewport = window->getViewport();

    this->font_view = std::make_unique<FontView>(this->kernel->getAssetManager());

    main_viewport->addView(this->font_view.get());

    auto gui = this->gui_system->getGUI("main");
    gui->getInputHandler().setButton(this->kernel->getInputManager()->getMouse()->getButton(0));

    return true;
}

void Application::cleanup() {
    this->kernel->shutdownSystems();
    this->kernel->getDisplayManager()->closeOpenWindows();
}

void Application::update(double time) {
}

void Application::handleEvent(const bembel::WindowShouldCloseEvent& event) {
    this->quit();
}

void Application::handleEvent(const bembel::WindowResizeEvent& event) {
}

void Application::handleEvent(const bembel::FileDropEvent& event) {
    this->converter->loade(event.files);
}
