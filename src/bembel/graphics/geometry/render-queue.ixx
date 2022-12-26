module;
#include <filesystem>
export module bembel.graphics.geometry:RenderQueue;

import bembel.base;
import bembel.kernel;
import :Material;
import :Mesh;
import :Model;
import :Renderer;

namespace bembel::graphics {
using namespace bembel::base;
using namespace bembel::kernel;

export class GeometryRenderQueue {
  public:
    GeometryRenderQueue();
    ~GeometryRenderQueue();

    bool addGeometryObject(GeometryModel* model, mat4 const& transform);

    // sorts the renderables to minimize state changes dureing rendering
    void sortRenderData();
    void clearRendarData();

    std::vector<GeometryRenderData> const& getRenderData() const { return m_render_data; }

  private:
    std::vector<GeometryRenderData> m_render_data;
};

} // namespace bembel::graphics
