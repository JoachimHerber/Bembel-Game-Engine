#include "selection-component.h"

#include <limits>

using namespace bembel;

const std::string& SelectionComponent::getComponentTypeName() {
    const static std::string typeName = "SelectionComponent";
    return typeName;
}

bool SelectionComponent::initComponent(
    kernel::AssetManager&, const base::xml::Element* elem, SelectionComponent* comp) {
    bool b;
    if(base::xml::getAttribute(elem, "selectable", b) && b) {
        comp->state = State::SELECTABLE;
    } else {
        comp->state = State::UNSELECTABLE;
    }

    return true;
}
