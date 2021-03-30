#ifndef BEMBEL_SELECTIONSHAPE_H
#define BEMBEL_SELECTIONSHAPE_H

#include <bembel/bembel.hpp>
#include <memory>

struct SelectionComponent {
    enum class State { UNSELECTABLE, SELECTABLE, FOCUSED, SELECTED };

    State state;

    using ContainerType = bembel::kernel::ComponentArray<SelectionComponent>;
    using ContainerPtr  = std::shared_ptr<ContainerType>;

    static const std::string& getComponentTypeName();
    static bool               initComponent(
                      bembel::kernel::AssetManager&, const bembel::base::xml::Element*, SelectionComponent*);
};
#endif // include guards
