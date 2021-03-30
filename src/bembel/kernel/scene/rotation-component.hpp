#ifndef BEMBEL_KERNEL_SCENE_ROTATION_COMPONENT_HPP
#define BEMBEL_KERNEL_SCENE_ROTATION_COMPONENT_HPP

#include "./component-container.hpp"
#include "./scene.hpp"

namespace bembel::kernel {

struct BEMBEL_API RotationComponent {
    glm::quat rotation;

    using ContainerType = ComponentArray<RotationComponent>;

    static const std::string& getComponentTypeName();
    static bool initComponent(AssetManager&, const base::xml::Element*, RotationComponent*);
};

} // namespace bembel::kernel
#endif // include guards
