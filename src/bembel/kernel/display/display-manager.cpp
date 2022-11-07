module;
#include <glbinding/gl/gl.h>
module bembel.kernel.display;


import glfw;

import bembel.base;
import bembel.kernel.assets;

import : Window;
import : Events;

namespace bembel::kernel {
using namespace bembel::base;
using namespace ::gl;

DisplayManager::DisplayManager(EventManager& event_mgr, AssetManager& asset_mgr) : m_event_mgr{event_mgr} {
    // glfwSetMonitorCallback(glfw_callbacks::MonitorCallback);
}

bool DisplayManager::createWindows(const base::xml::Element* properties) {
    if(!properties) return false;

    for(auto window_properties : base::xml::IterateChildElements(properties, "Window")) {
        auto window = std::make_unique<Window>(m_event_mgr, WindowId(m_windows.size()));
        window->init(window_properties);
        m_windows.push_back(std::move(window));
    }
    return true;
}

void DisplayManager::closeOpenWindows() {
    for(auto& window : m_windows) {
        if(window->isOpend()) window->close();
    }
}

void DisplayManager::updateWindows() {
    for(auto& window : m_windows) {
        if(!window->isOpend()) continue;

        window->makeContextCurent();

        glDisable(GL_SCISSOR_TEST);
        const glm::vec3& bg = window->getBackgroundColor();
        glClearColor(bg.r, bg.g, bg.b, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for(const auto& viewport : window->getViewports()) { viewport->draw(); }

        m_event_mgr.broadcast(WindowUpdateEvent{window.get()});

        window->swapBuffers();
    }
}

} // namespace bembel::kernel
