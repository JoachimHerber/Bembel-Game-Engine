module;
#include <glbinding/gl/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <Windows.h>
module bembel.kernel.core;

import bembel.base;
import bembel.kernel.assets;

extern "C" {
__declspec(dllexport) DWORD NvOptimusEnablement                = 1;
__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

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
    glfwSetErrorCallback(glfw_error_callback);

    this->input.keyboard.initDefaultKeys();

    CursorIcon::createStandardCursors(this->assets);
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
    if(asset_dirs) this->assets.getAssetLocator().init(asset_dirs);

    xml::Element const* display = root->FirstChildElement("Display");
    if(display) this->display.createWindows(display);

    xml::Element const* systems = root->FirstChildElement("Systems");
    if(systems) {
        for(auto& system : m_systems) {
            xml::Element const* system_config = systems->FirstChildElement(system->name.c_str());

            if(!system->configure(system_config)) {
                log().error("Failed to init '{}'-System from file '{}'", system->name, file_path);
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
