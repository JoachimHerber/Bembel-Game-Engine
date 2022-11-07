module;
#include <glbinding/gl/gl.h>

#include "bembel/pch.h"
module bembel.kernel.core;

import glfw;

import bembel.base;
import bembel.kernel.assets;

namespace bembel::kernel {
using namespace bembel::base;
using namespace ::gl;

void glfw_error_callback(int error, char const* description) {
    log().error("GLFW-Error({}): {}", error, description);
}

Engine::Engine() {
    if(glfwInit() == GL_FALSE) {
        log().error("Failed to initialize GLFW");
        throw std::exception();
    }

    m_display_manager = std::make_unique<DisplayManager>(m_event_manager, m_asset_manager);
    m_input_manager   = std::make_unique<InputManager>(m_event_manager);

    glfwSetErrorCallback(glfw_error_callback);

    CursorIcon::createStandardCursors(m_asset_manager);
}

Engine::~Engine() {
    // delete system in revers order of their creation
    for(size_t n = m_systems.size(); n-- > 0;) m_systems[n].reset();

    glfwTerminate();
}

bool Engine::removeSystem(std::string_view name) {
    auto it = m_system_mapping.find(name);
    if(it == m_system_mapping.end()) {
        log().warning("Unknown system '{}'", name);
        return false;
    }

    m_systems[it->second].reset();
    m_system_mapping.erase(it);
    return true;
}

System* Engine::getSystem(std::string_view name) {
    auto it = m_system_mapping.find(name);
    if(it != m_system_mapping.end()) return m_systems[it->second].get();

    log().warning("Unknown system '{}'", name);
    return nullptr;
}

bool Engine::initSystems() {
    for(auto& system : m_systems) {
        if(!system->init()) return false;
    }
    return true;
}

void Engine::updateSystems(double time_since_last_update) {
    for(auto& system : m_systems) system->update(time_since_last_update);
}

void Engine::shutdownSystems() {
    for(auto& system : m_systems) system->shutdown();
}

bool Engine::loadSetting(std::filesystem::path file) {
    std::string const file_path = file.string(); // file.c_str() returns a wchar*
    xml::Document     doc;
    if(doc.LoadFile(file_path.c_str()) != tinyxml2::XML_SUCCESS) {
        log().error("Failed to lode file '{}'\n{}", file_path, doc.ErrorName());
        return false;
    }

    xml::Element const* root = doc.FirstChildElement("Bembel");
    if(!root) {
        log().error("Config file '{}' has no root element 'Bembel'", file_path);
        return false;
    }

    xml::Element const* asset_dirs = root->FirstChildElement("AssetDirectories");
    if(asset_dirs) m_asset_manager.getAssetLocator().init(asset_dirs);

    xml::Element const* display = root->FirstChildElement("Display");
    if(display) m_display_manager->createWindows(display);

    xml::Element const* systems = root->FirstChildElement("Systems");
    if(systems) {
        for(auto& system : m_systems) {
            std::string         system_name   = system->getName();
            xml::Element const* system_config = systems->FirstChildElement(system_name.c_str());

            if(!system->configure(system_config)) {
                log().error("Failed to init '{}'-System from file '{}'", system_name, file_path);
                return false;
            }
        }
    }
    return true;
}

void Engine::pollEvents() {
    glfwPollEvents();
}

} // namespace bembel::kernel
