﻿module;
#include <glbinding/gl/gl.h>

#include <memory>
module bembel.kernel.display;

import bembel.base;
import bembel.kernel.assets;

import :Window;
import :Events;

namespace bembel::kernel {
using namespace bembel::base;
using namespace ::gl;

DisplayManager::DisplayManager() {
    // glfwSetMonitorCallback(glfw_callbacks::MonitorCallback);
}

bool DisplayManager::createWindows(const base::xml::Element* properties) {
    if(!properties) return false;

    for(auto window_properties : base::xml::IterateChildElements(properties, "Window")) {
        auto window = std::make_unique<Window>(WindowId(m_windows.size()));
        window->init(window_properties, m_windows.empty() ? nullptr : m_windows[0].get());
        m_windows.push_back(std::move(window));
    }
    if(m_windows.size() > 1) { m_windows[0]->makeContextCurent(); }
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
        vec3 const bg = window->background_color;
        glClearColor(bg.r, bg.g, bg.b, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for(const auto& viewport : window->getViewports()) { //
            viewport->draw();
        }

        events::broadcast(WindowUpdateEvent{window.get()});

        window->swapBuffers();
    }
    if(m_windows.size() > 1) { m_windows[0]->makeContextCurent(); }
}

} // namespace bembel::kernel
