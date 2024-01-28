module;
#include <GLFW/glfw3.h>

#include <filesystem>

#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "imgui.h"
module bembel.gui;

import bembel.base;
import bembel.kernel;
import bembel.gui.core;
import bembel.gui.widgets;

namespace bembel::gui {
using namespace bembel::base;
using namespace bembel::kernel;

GuiSystem::GuiSystem(Engine& engine) : System{"UserInterface"}, m_engine{engine} {
    assets::registerAssetType<Shader>();
    assets::registerAssetType<ShaderProgram>();
    assets::registerAssetType<TextureAtlas>();
    assets::registerAssetType<Texture>();
    assets::registerAssetType<SdfFont>();
    assets::registerAssetType<Style>();

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
    }
    return true;
}

bool GuiSystem::init() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // IF using Docking Branch

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(m_engine.display.getWindow(0)->getGlfwWindow(), true);
    ImGui_ImplOpenGL3_Init();
    return true;
}

void GuiSystem::shutdown() {
    m_guis.clear();
    m_named_guis.clear();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void GuiSystem::update(double) {}

GraphicalUserInterface* GuiSystem::createGUI(std::string_view name) {
    if(!name.empty() && m_named_guis.find(name) != m_named_guis.end()) {
        logError(
            "Can't create GUI with name '{}'. A GUI with the same name already exists.", name
        );
        return nullptr;
    }

    m_guis.push_back(std::make_unique<GraphicalUserInterface>(m_engine));

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
