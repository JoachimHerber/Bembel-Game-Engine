module;
#include <filesystem>
#include <string>
#include <string_view>
module bembel.examples.gui;

import bembel.base;

namespace bembel {

using namespace bembel::base;
using namespace bembel::kernel;
using namespace bembel::gui;

GuiExample::GuiExample() : Application() {
    m_gui_system = m_engine.addSystem<GuiSystem>();

    events::addHandler<WindowShouldCloseEvent>(this);
}

GuiExample::~GuiExample() {
    events::removeHandler<WindowShouldCloseEvent>(this);
}

bool GuiExample::init(std::span<std::string_view>) {
    if(!m_engine.loadSetting("GUI/config.xml")) return false;

    m_engine.initSystems();

    auto gui = m_gui_system->getGUI("main");

    auto button_1 = gui->getWidget<ButtonWidget>("MainWindow/Body/Button_1");
    auto button_2 = gui->getWidget<ButtonWidget>("MainWindow/Body/Button_2");
    auto button_3 = gui->getWidget<ButtonWidget>("MainWindow/Body/Button_3");

    if(button_1) { button_1->click_signal.bind(this, &GuiExample::onButton1Click); }
    if(button_2) { button_2->click_signal.bind(this, &GuiExample::onButton2Click); }
    if(button_3) { button_3->click_signal.bind(this, &GuiExample::onButton3Click); }

    return true;
}

void GuiExample::cleanup() {
    m_engine.shutdownSystems();
    m_engine.display.closeOpenWindows();
}

void GuiExample::update(double time) {}

void GuiExample::handleEvent(const WindowShouldCloseEvent& event) {
    quit();
}

void GuiExample::onButton1Click() {
    log().info("Button 1 clicked");
}

void GuiExample::onButton2Click() {
    log().info("Button 2 clicked");
}

void GuiExample::onButton3Click() {
    log().info("Button 3 clicked");
}

} // namespace bembel
