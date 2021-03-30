#include "./gui-system.hpp"

#include <bembel/base/io/xml.hpp>
#include <bembel/base/logging/logger.hpp>
#include <bembel/kernel/assets/asset-manager.hpp>
#include <bembel/kernel/core/kernel.hpp>
#include <bembel/kernel/display/display-manager.hpp>
#include <bembel/kernel/display/window.hpp>
#include <bembel/kernel/rendering/shader.hpp>
#include <bembel/kernel/rendering/texture-atlas.hpp>

#include "./graphical-user-interface.hpp"
#include "./rendering/style.hpp"

namespace bembel::gui {

GUISystem::GUISystem(kernel::Kernel& kernel)
: System(kernel, "UserInterface") {
    kernel.getAssetManager().registerAssetType<kernel::Shader>();
    kernel.getAssetManager().registerAssetType<kernel::ShaderProgram>();
    kernel.getAssetManager().registerAssetType<kernel::TextureAtlas>();
    kernel.getAssetManager().registerAssetType<kernel::Texture>();
    kernel.getAssetManager().registerAssetType<kernel::Font>();
    kernel.getAssetManager().registerAssetType<Style>();
}
GUISystem::~GUISystem() {
}

bool GUISystem::configure(const base::xml::Element* properties) {
    if(!properties) return false;

    for(auto properties : base::xml::IterateChildElements(properties, "GUI")) {
        std::string name = "";
        base::xml::getAttribute(properties, "name", name);
        auto gui = createGUI(name);

        if(!gui->init(properties)) return false;

        unsigned windowId, viewportId;
        if(base::xml::getAttribute(properties, "window", windowId)
           && base::xml::getAttribute(properties, "viewport", viewportId)) {
            auto& display_mgr = this->getKernel().getDisplayManager();
            auto  window      = display_mgr.getWindow(windowId);

            if(window && window->getViewports().size() > viewportId) {
                window->getViewports()[viewportId]->addView(gui->getView());
            }
        }
    }
    return true;
}

bool GUISystem::init() {
    return true;
}

void GUISystem::shutdown() {
    this->named_guis.clear();
    this->guis.clear();
}

void GUISystem::update(double) {
}

GraphicalUserInterface* GUISystem::createGUI(const std::string& name) {
    if(!name.empty() && this->named_guis.find(name) != this->named_guis.end()) {
        BEMBEL_LOG_ERROR() << "Can't create GUI with name '" << name
                           << "', sice a GUI with the same name already exists.";
        return nullptr;
    }

    this->guis.push_back(std::make_unique<GraphicalUserInterface>(
        this->kernel.getEventManager(), this->kernel.getAssetManager()));

    GraphicalUserInterface* gui = this->guis.back().get();

    if(!name.empty()) this->named_guis.emplace(name, gui);

    return gui;
}

GraphicalUserInterface* GUISystem::getGUI(const std::string& name) {
    auto it = this->named_guis.find(name);
    return it != this->named_guis.end() ? it->second : nullptr;
}

} // namespace bembel::gui
