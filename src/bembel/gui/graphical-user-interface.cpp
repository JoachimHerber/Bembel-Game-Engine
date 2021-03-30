#include "./graphical-user-interface.hpp"

#include <bembel/base/logging/logger.hpp>
#include <bembel/kernel/display/window.hpp>

#include "./widgets/group-widget.hpp"

namespace bembel::gui {

GraphicalUserInterface::GraphicalUserInterface(
    kernel::EventManager& event_mgr, kernel::AssetManager& asset_mgr)
: event_mgr{event_mgr}
, asset_mgr{asset_mgr}
, view{this}
, renderer{*this}
, input_handler{this}
, root_widget{std::make_unique<GroupWidget>(this, nullptr)} {
}
GraphicalUserInterface::~GraphicalUserInterface() {
}

bool GraphicalUserInterface::init(const base::xml::Element* properties, bool load_file) {
    if(!properties) return false;

    std::string file_name;
    if(load_file && base::xml::getAttribute(properties, "config_file", file_name)) {
        return this->init(file_name);
    } else {
        if(!this->renderer.init(properties->FirstChildElement("Renderer"))) { return false; }

        return initWidgets(properties);
    }
}

bool GraphicalUserInterface::init(const std::string& file_name) {
    base::xml::Document doc;
    if(doc.LoadFile(file_name.c_str()) != tinyxml2::XML_SUCCESS) return false;

    auto properties = doc.FirstChildElement("GUI");

    return this->init(properties, false);
}

Widget* GraphicalUserInterface::getWidget(const std::string& widgetName) const {
    auto it = this->named_widgets.find(widgetName);
    if(it != this->named_widgets.end()) return it->second;
    return nullptr;
}

Renderer& GraphicalUserInterface::getRenderer() {
    return this->renderer;
}

InputHandler& GraphicalUserInterface::getInputHandler() {
    return this->input_handler;
}

GroupWidget* GraphicalUserInterface::getRootWidget() const {
    return this->root_widget.get();
}

kernel::EventManager& GraphicalUserInterface::getEventManager() {
    return this->event_mgr;
}

kernel::AssetManager& GraphicalUserInterface::getAssetManager() {
    return this->asset_mgr;
}

bool GraphicalUserInterface::initWidgets(const base::xml::Element* properties) {
    auto widgets = properties->FirstChildElement("Widgets");
    return this->root_widget->init(widgets);
}

void GraphicalUserInterface::View::onCurserMove(const glm::vec2& pos) {
    this->gui->input_handler.onCursorMoved(pos);
}

void GraphicalUserInterface::View::onResize(const glm::uvec2& size) {
    this->gui->root_widget->size.set(size);
}

void GraphicalUserInterface::View::draw(const glm::ivec2& pos, const glm::uvec2& size) {
    this->gui->renderer.drawGui(pos, size);
}

} // namespace bembel::gui
