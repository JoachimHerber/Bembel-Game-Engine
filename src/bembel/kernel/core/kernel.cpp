#include "./kernel.hpp"

#include <glfw/glfw3.h>

#include <bembel/base/io/xml.hpp>
#include <bembel/base/logging/logger.hpp>

#include "../assets/asset-manager.hpp"
#include "../display/display-manager.hpp"
#include "../events/event-manager.hpp"
#include "../input/input-manager.hpp"
#include "./system.hpp"

namespace bembel::kernel {
namespace xml = bembel::base::xml;

Kernel::Kernel() {
    if(glfwInit() == GL_FALSE) {
        BEMBEL_LOG_ERROR() << "Failed to initialize GLFW" << std::endl;
        throw std::exception();
    }
}

Kernel::~Kernel() {
    // delete system in revers order of their creation
    for(size_t n = this->systems.size(); n-- > 0;) this->systems[n].reset();

    glfwTerminate();
}

EventManager& Kernel::getEventManager() {
    return this->event_manager;
}
const EventManager& Kernel::getEventManager() const {
    return this->event_manager;
}

AssetManager& Kernel::getAssetManager() {
    return this->asset_manager;
}
const AssetManager& Kernel::getAssetManager() const {
    return this->asset_manager;
}

DisplayManager& Kernel::getDisplayManager() {
    return this->display_manager;
}
const DisplayManager& Kernel::getDisplayManager() const {
    return this->display_manager;
}

InputManager& Kernel::getInputManager() {
    return this->input_manager;
}
const InputManager& Kernel::getInputManager() const {
    return this->input_manager;
}

bool Kernel::removeSystem(const std::string& name) {
    auto it = this->system_mapping.find(name);
    if(it == this->system_mapping.end()) {
        BEMBEL_LOG_WARNING() << "Unknown system '" << name << "'." << std::endl;
        return false;
    }

    this->systems[it->second].reset();
    this->system_mapping.erase(it);
    return true;
}

System* Kernel::getSystem(const std::string& name) {
    auto it = this->system_mapping.find(name);
    if(it != this->system_mapping.end()) return this->systems[it->second].get();

    BEMBEL_LOG_WARNING() << "Unknown system '" << name << "'." << std::endl;
    return nullptr;
}

bool Kernel::initSystems() {
    for(auto& system : this->systems) {
        if(!system->init()) return false;
    }
    return true;
}

void Kernel::updateSystems(double time_since_last_update) {
    for(auto& system : this->systems) system->update(time_since_last_update);
}
void Kernel::shutdownSystems() {
    for(auto& system : this->systems) system->shutdown();
}

bool Kernel::loadSetting(const std::string& config_file_name) {
    xml::Document doc;
    if(doc.LoadFile(config_file_name.c_str()) != tinyxml2::XML_SUCCESS) {
        BEMBEL_LOG_ERROR() << "Failed to lode file '" << config_file_name << "'\n"
                           << doc.ErrorName() << std::endl;
        return false;
    }

    const xml::Element* root = doc.FirstChildElement("Bembel");
    if(!root) {
        BEMBEL_LOG_ERROR() << "Config file '" << config_file_name
                           << "' has no root element 'Bembel'\n"
                           << std::endl;
        return false;
    }

    const xml::Element* asset_dirs = root->FirstChildElement("AssetDirectories");
    if(asset_dirs) this->asset_manager.getAssetLocator().init(asset_dirs);

    const xml::Element* display = root->FirstChildElement("Display");
    if(display) this->display_manager.createWindows(display);

    const xml::Element* systems = root->FirstChildElement("Systems");
    if(systems) {
        for(auto& system : this->systems) {
            const xml::Element* system_config =
                systems->FirstChildElement(system->getName().c_str());

            if(!system->configure(system_config)) {
                BEMBEL_LOG_ERROR() << "Failed to init '" << system->getName()
                                   << "'-System from file '" << config_file_name << "'\n"
                                   << std::endl;
                return false;
            }
        }
    }
    return true;
}

void Kernel::pollEvents() {
    glfwPollEvents();
}

} // namespace bembel::kernel
