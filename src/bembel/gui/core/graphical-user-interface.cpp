module;
#include <filesystem>
module bembel.gui.core;

import bembel.base;
import bembel.kernel;
import :RenderBatch;
import :InteractionHandle;
import :GroupWidget;

namespace bembel::gui {
using namespace bembel::base;
using namespace bembel::kernel;

GraphicalUserInterface::GraphicalUserInterface(Engine& engine)
  : engine{engine}
  , view{*this}
  , input{root_widget, view}
  , renderer{root_widget}
  , root_widget{*this} {
    this->root_widget.setName("Root");
}

GraphicalUserInterface::~GraphicalUserInterface() {}

bool GraphicalUserInterface::init(xml::Element const* properties, bool load_file) {
    if(!properties) return false;

    std::string file_name;
    if(load_file && xml::getAttribute(properties, "config_file", file_name)) {
        return init(file_name);
    } else {
        if(!this->renderer.init(properties->FirstChildElement("Renderer"))) {
            logError("Failed to init Renderer");
            return false;
        }
        if(!this->root_widget.configure(properties->FirstChildElement("Widgets"))) {
            logError("Failed to init Widgets");
            return false;
        }
    }

    unsigned windowId, viewportId;
    if(xml::getAttribute(properties, "window", windowId)
       && xml::getAttribute(properties, "viewport", viewportId)) {
        auto window = engine.display.getWindow(windowId);

        if(window && window->getViewports().size() > viewportId) {
            window->getViewports()[viewportId]->addView(&this->view);
        }
    }

    return true;
}

bool GraphicalUserInterface::init(std::filesystem::path file) {
    std::string const file_path = file.string(); // file.c_str() returns a wchar*
    xml::Document     doc;
    if(doc.LoadFile(file_path.c_str()) != tinyxml2::XML_SUCCESS) {
        logError("Failed load file '{}' - Error: {}", file_path, doc.ErrorName());
        return false;
    }

    auto properties = doc.FirstChildElement("GUI");

    return init(properties, false);
}

Widget* GraphicalUserInterface::getWidget(std::string_view path) const {
    return this->root_widget.getChildWidget(path);
}

void GraphicalUserInterface::View::onCurserMove(In<vec2> pos) {
    m_gui.input.onCursorMoved(pos);
}

void GraphicalUserInterface::View::onResize(In<uvec2> size) {
    m_gui.root_widget.size.set(size);
}

void GraphicalUserInterface::View::draw(In<ivec2> pos, In<uvec2> size) {
    m_gui.renderer.drawGui(pos, size);
}

} // namespace bembel::gui
