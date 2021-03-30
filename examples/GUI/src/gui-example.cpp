/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/
#include "gui-example.h"

namespace bembel {

GUIExample::GUIExample()
: kernel::Application() {
    this->gui_system = this->kernel->addSystem<gui::GUISystem>();

    this->kernel->getEventManager().addHandler<kernel::WindowShouldCloseEvent>(this);
}

GUIExample::~GUIExample() {
}

bool GUIExample::init() {
    if(!this->kernel->loadSetting("GUI/config.xml")) return false;

    this->kernel->initSystems();

    auto gui = this->gui_system->getGUI("main");
    gui->getInputHandler().setButton(this->kernel->getInputManager().getMouse().getButton(0));

    auto button_1 = gui->getWidget<gui::LabeledButtonWidget>("Button_1");
    auto button_2 = gui->getWidget<gui::LabeledButtonWidget>("Button_2");
    auto button_3 = gui->getWidget<gui::LabeledButtonWidget>("Button_3");

    if(button_1) { button_1->click_signal.addSlot(this, &GUIExample::onButton1Click); }
    if(button_2) { button_2->click_signal.addSlot(this, &GUIExample::onButton2Click); }
    if(button_3) { button_3->click_signal.addSlot(this, &GUIExample::onButton3Click); }

    return true;
}

void GUIExample::cleanup() {
    this->kernel->shutdownSystems();
    this->kernel->getDisplayManager().closeOpenWindows();
}

void GUIExample::update(double time) {
}

void GUIExample::handleEvent(const kernel::WindowShouldCloseEvent& event) {
    quit();
}

void GUIExample::onButton1Click() {
    BEMBEL_LOG_INFO() << "Button 1 clicked";
}

void GUIExample::onButton2Click() {
    BEMBEL_LOG_INFO() << "Button 2 clicked";
}

void GUIExample::onButton3Click() {
    BEMBEL_LOG_INFO() << "Button 3 clicked";
}

} // namespace bembel
