#ifndef BEMBEL_GRAPHICS_GEOMETRY_RENDERQUEUE_HPP
#define BEMBEL_GRAPHICS_GEOMETRY_RENDERQUEUE_HPP

#include <bembel/base/io/xml.hpp>
#include <bembel/kernel/assets/asset-handle.hpp>
#include <bembel/kernel/assets/asset-manager.hpp>
#include <bembel/kernel/open-gl.hpp>

#include "./renderer.hpp"

namespace bembel::graphics {

class GeometryModel;
class Material;

class BEMBEL_API GeometryRenderQueue {
  public:
    GeometryRenderQueue(kernel::AssetManager& asset_mgr);
    ~GeometryRenderQueue();

    bool addGeometryObject(kernel::AssetHandle model, const glm::mat4& transform);
    bool addGeometryObject(GeometryModel* model, const glm::mat4& transform);

    // sorts the renderables to minimize state changes dureing rendering
    void                                   sortRenderData();
    void                                   clearRendarData();
    const std::vector<GeometryRenderData>& getRenderData() const { return this->render_data; }

  private:
    kernel::AssetManager&           asset_mgr;
    std::vector<GeometryRenderData> render_data;
};

} // namespace bembel::graphics
#endif // include guards
