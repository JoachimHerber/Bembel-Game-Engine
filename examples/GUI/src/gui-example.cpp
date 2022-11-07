module;
#include <string>
module bembel.examples.gui;

import bembel.base;

namespace bembel {

using namespace bembel::base;
using namespace bembel::kernel;
using namespace bembel::gui;

GuiExample::GuiExample() : Application() {
    m_gui_system = m_engine.addSystem<GuiSystem>();

    m_engine.getEventManager().addHandler<WindowShouldCloseEvent>(this);
}

GuiExample::~GuiExample() {}

bool GuiExample::init() {
    if(!m_engine.loadSetting("GUI/config.xml")) return false;

    m_engine.initSystems();

    auto gui = m_gui_system->getGUI("main");

    auto button_1 = gui->getWidget<LabeledButtonWidget>("MainWindow/Body/Button_1");
    auto button_2 = gui->getWidget<LabeledButtonWidget>("MainWindow/Body/Button_2");
    auto button_3 = gui->getWidget<LabeledButtonWidget>("MainWindow/Body/Button_3");

    if(button_1) { button_1->click_signal.bind(this, &GuiExample::onButton1Click); }
    if(button_2) { button_2->click_signal.bind(this, &GuiExample::onButton2Click); }
    if(button_3) { button_3->click_signal.bind(this, &GuiExample::onButton3Click); }

    return true;
}

void GuiExample::cleanup() {
    m_engine.shutdownSystems();
    m_engine.getDisplayManager().closeOpenWindows();
}

void GuiExample::update(double time) {}

void GuiExample::handleEvent(const WindowShouldCloseEvent& event) { quit(); }

void GuiExample::onButton1Click() { log().info("Button 1 clicked"); }

void GuiExample::onButton2Click() { log().info("Button 2 clicked"); }

void GuiExample::onButton3Click() { log().info("Button 3 clicked"); }

} // namespace bembel