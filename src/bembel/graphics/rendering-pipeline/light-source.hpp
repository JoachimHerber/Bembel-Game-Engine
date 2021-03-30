#ifndef BEMBEL_GRAPHICS_RENDERING_PIPELINE_LIGHTSOURCE_HPP
#define BEMBEL_GRAPHICS_RENDERING_PIPELINE_LIGHTSOURCE_HPP

#include <bembel/kernel/scene/component-container.hpp>
#include <bembel/kernel/scene/entity.hpp>
#include <bembel/kernel/scene/scene.hpp>

namespace bembel::graphics {

class BEMBEL_API PointLightSource {
  public:
    glm::vec3 color;

    float bulb_radius;
    float cutoff_radius;

    using ContainerType = kernel::ComponentMap<PointLightSource>;
    using ContainerPtr  = std::shared_ptr<ContainerType>;

    static const std::string& getComponentTypeName();
    static bool initComponent(kernel::AssetManager&, const base::xml::Element*, PointLightSource*);
};

class BEMBEL_API DirectionalLightSource {
  public:
    glm::vec3 color;
    glm::vec3 direction;

    using ContainerType = kernel::ComponentMap<DirectionalLightSource>;
    using ContainerPtr  = std::shared_ptr<ContainerType>;

    static const std::string& getComponentTypeName();
    static bool               initComponent(
                      kernel::AssetManager&, const base::xml::Element*, DirectionalLightSource*);
};

} // namespace bembel::graphics
#endif // include guards
