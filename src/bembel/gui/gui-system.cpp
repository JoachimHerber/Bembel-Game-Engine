module;
#include <GLFW/glfw3.h>

#include "bembel/pch.h"
module bembel.gui;

import bembel.base;
import bembel.kernel;
import bembel.gui.core;

namespace bembel::gui {
using namespace bembel::base;
using namespace bembel::kernel;

GuiSystem::GuiSystem(Engine& engine) : System{"UserInterface"}, m_engine{engine} {
    engine.assets.registerAssetType<Shader>();
    engine.assets.registerAssetType<ShaderProgram>();
    engine.assets.registerAssetType<TextureAtlas>();
    engine.assets.registerAssetType<Texture>();
    engine.assets.registerAssetType<Font>();
    engine.assets.registerAssetType<Style>();

    registerWidgetTypesInFactory();
}
GuiSystem::~GuiSystem() {}

bool GuiSystem::configure(xml::Element const* properties) {
    if(!properties) return false;

    for(auto properties : xml::IterateChildElements(properties, "GUI")) {
        std::string name = "";
        xml::getAttribute(properties, "name", name);
        auto gui = createGUI(name);

        if(!gui->init(properties)) return false;

        unsigned windowId, viewportId;
        if(xml::getAttribute(properties, "window", windowId)
           && xml::getAttribute(properties, "viewport", viewportId)) {
            auto window = m_engine.display.getWindow(windowId);

            if(window && window->getViewports().size() > viewportId) {
                window->getViewports()[viewportId]->addView(&gui->view);
            }
        }
    }
    return true;
}

bool GuiSystem::init() {
    return true;
}

void GuiSystem::shutdown() {
    m_guis.clear();
    m_named_guis.clear();
}

void GuiSystem::update(double) {}

GraphicalUserInterface* GuiSystem::createGUI(std::string_view name) {
    if(!name.empty() && m_named_guis.find(name) != m_named_guis.end()) {
        log().error(
            "Can't create GUI with name '{}'. A GUI with the same name already exists.", name
        );
        return nullptr;
    }

    m_guis.push_back(std::make_unique<GraphicalUserInterface>(m_engine.assets));

    GraphicalUserInterface* gui = m_guis.back().get();

    gui->input.setButtons(
        m_engine.input.mouse.getButton(0),
        m_engine.input.keyboard.getKey(Keyboard::DELETE),
        m_engine.input.keyboard.getKey(Keyboard::BACKSPACE),
        m_engine.input.keyboard.getKey(Keyboard::RIGHT),
        m_engine.input.keyboard.getKey(Keyboard::LEFT),
        m_engine.input.keyboard.getKey(Keyboard::UP),
        m_engine.input.keyboard.getKey(Keyboard::DOWN)
    );

    if(!name.empty()) m_named_guis.emplace(name, gui);

    return gui;
}

GraphicalUserInterface* GuiSystem::getGUI(std::string_view name) {
    auto it = m_named_guis.find(name);
    return it != m_named_guis.end() ? it->second : nullptr;
}

} // namespace bembel::gui
