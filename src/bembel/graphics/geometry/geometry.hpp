#ifndef BEMBEL_GRAPHICS_GEOMETRY_COMPONENT_HPP
#define BEMBEL_GRAPHICS_GEOMETRY_COMPONENT_HPP

#include <bembel/kernel/assets/asset-handle.hpp>
#include <bembel/kernel/scene/component-container.hpp>
#include <bembel/kernel/scene/entity.hpp>
#include <bembel/kernel/scene/scene.hpp>

namespace bembel::graphics {

class BEMBEL_API GeometryComponent {
  public:
    kernel::AssetHandle model;

    // the ID of the renderer that should be used for rendering the geometry
    unsigned renderer;

    using ContainerType = kernel::ComponentArray<GeometryComponent>;

    static const std::string& getComponentTypeName();
    static bool               initComponent(
                      kernel::AssetManager&     asset_mgr,
                      const base::xml::Element* properties,
                      GeometryComponent*        geometry_component);
};

} // namespace bembel::graphics
#endif // include guards
