#ifndef BEMBEL_KERNEL_SCENE_POSITIONCOMPONENT_HPP
#define BEMBEL_KERNEL_SCENE_POSITIONCOMPONENT_HPP

#include "./component-container.hpp"
#include "./scene.hpp"

namespace bembel::kernel {

struct BEMBEL_API PositionComponent {
    // This Component stores the 3D position of an entity
    glm::vec3 position;

    using ContainerType = ComponentArray<PositionComponent>;

    static const std::string& getComponentTypeName();
    static bool initComponent(AssetManager&, const base::xml::Element*, PositionComponent*);
};

} // namespace bembel::kernel
#endif // include guards
