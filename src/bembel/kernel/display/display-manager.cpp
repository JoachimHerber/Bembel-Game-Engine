#include "./display-manager.hpp"

#include <glfw/glfw3.h>

#include <bembel/base/logging/logger.hpp>

#include "../assets/asset-manager.hpp"
#include "../core/kernel.hpp"
#include "../events/display-events.hpp"
#include "../events/event-callbacks.hpp"
#include "./cursor-icon.hpp"
#include "./window.hpp"

namespace bembel::kernel {

DisplayManager::DisplayManager(EventManager& event_mgr, AssetManager& asset_mgr)
: event_mgr{event_mgr} {
    // glfwSetMonitorCallback(glfw_callbacks::MonitorCallback);

    this->display_mode_factory.registerDefaultObjectGenerator<WindowDisplayMode>("Windowed");
    this->display_mode_factory.registerDefaultObjectGenerator<FullscreenDisplayMode>("Fullscreen");

    asset_mgr.registerAssetType<CursorIcon>();

    asset_mgr.addAsset(
        std::make_unique<CursorIcon>(glfwCreateStandardCursor(GLFW_ARROW_CURSOR)), "Arrow");
    asset_mgr.addAsset(
        std::make_unique<CursorIcon>(glfwCreateStandardCursor(GLFW_IBEAM_CURSOR)), "IBeam");
    asset_mgr.addAsset(
        std::make_unique<CursorIcon>(glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR)), "Crosshair");
    asset_mgr.addAsset(
        std::make_unique<CursorIcon>(glfwCreateStandardCursor(GLFW_HAND_CURSOR)), "Hand");
    asset_mgr.addAsset(
        std::make_unique<CursorIcon>(glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR)), "HResize");
    asset_mgr.addAsset(
        std::make_unique<CursorIcon>(glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR)), "VResize");
}

DisplayManager::~DisplayManager() {
    deleteAllWindows();
}

bool DisplayManager::createWindows(const base::xml::Element* properties) {
    if(!properties) return false;

    for(auto window_properties : xml::IterateChildElements(properties, "Window")) {
        this->windows.push_back(std::make_unique<Window>(*this, unsigned(this->windows.size())));
        this->windows.back()->init(window_properties);
    }
    return true;
}

void DisplayManager::closeOpenWindows() {
    while(!this->open_windows.empty()) this->open_windows.front()->close();
}

void DisplayManager::deleteAllWindows() {
    closeOpenWindows();
    this->windows.clear();
}

void DisplayManager::updateWindows() {
    for(Window* window : this->open_windows) {
        window->makeContextCurent();

        glDisable(GL_SCISSOR_TEST);
        const glm::vec3& bg = window->getBackgroundColor();
        glClearColor(bg.r, bg.g, bg.b, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for(const auto& viewport : window->getViewports()) { viewport->draw(); }

        this->event_mgr.broadcast(WindowUpdateEvent{window});

        window->swapBuffers();
    }
}

void DisplayManager::onWindowOpend(Window* window) {
    this->open_windows.push_back(window);
    GLFWwindow* glfw = window->getGlfwWindow();

    glfwSetWindowUserPointer(glfw, window);

    using namespace event_callbacks;
    glfwSetWindowPosCallback(glfw, windowPositionCallback);
    glfwSetWindowSizeCallback(glfw, windowSizeCallback);
    glfwSetWindowCloseCallback(glfw, windowCloseCallback);
    glfwSetWindowRefreshCallback(glfw, windowRefreshCallback);
    glfwSetWindowFocusCallback(glfw, windowFocusCallback);
    glfwSetWindowIconifyCallback(glfw, windowIconifyCallback);

    glfwSetFramebufferSizeCallback(glfw, framebufferSizeCallback);

    glfwSetKeyCallback(glfw, keyCallback);
    glfwSetCharCallback(glfw, charCallback);
    glfwSetCharModsCallback(glfw, charModsCallback);

    glfwSetMouseButtonCallback(glfw, mouseButtonCallback);
    glfwSetCursorPosCallback(glfw, cursorPositionCallback);
    glfwSetCursorEnterCallback(glfw, cursorEnterCallback);
    glfwSetScrollCallback(glfw, scrollCallback);

    glfwSetDropCallback(glfw, dropCallback);

    this->event_mgr.broadcast(WindowOpendEvent{window});
}

void DisplayManager::onWindowClosed(Window* window) {
    auto it = std::find(this->open_windows.begin(), this->open_windows.end(), window);
    if(it != this->open_windows.end()) {
        this->open_windows.erase(it);
        this->event_mgr.broadcast(WindowClosedEvent{window});
    }
}

Window* DisplayManager::createWindow() {
    this->windows.push_back(std::make_unique<Window>(*this, unsigned(this->windows.size())));
    return this->windows.back().get();
}

Window* DisplayManager::getWindow(unsigned id) const {
    if(id < this->windows.size()) return this->windows[id].get();

    return nullptr;
}

unsigned DisplayManager::getNumWindows() const {
    return unsigned(this->windows.size());
}

EventManager& DisplayManager::getEventManager() const {
    return this->event_mgr;
}

DisplayManager::DisplayModeFactory& DisplayManager::getDisplayModeFactory() {
    return this->display_mode_factory;
}

} // namespace bembel::kernel
