module;
#include <algorithm>
#include <string_view>
module bembel.graphics.geometry;

import bembel.base;
import bembel.kernel;

namespace bembel::graphics {
using namespace bembel::base;
using namespace bembel::kernel;

GeometryRenderQueue::GeometryRenderQueue() {}

GeometryRenderQueue::~GeometryRenderQueue() {}

bool GeometryRenderQueue::addGeometryObject(GeometryModel* model, mat4 const& transform) {
    if(model == nullptr) return false;

    auto mesh = model->getMesh();
    if(mesh == nullptr) return false;

    for(auto const& mat_map : model->getMateialMapping()) {
        Material* mat = mat_map.material.get();
        if(mesh == nullptr) continue;

        auto sub_mesh = mesh->getSubMesh(mat_map.sub_mesh);
        if(sub_mesh.has_value()) {
            m_render_data.emplace_back(mesh, mat, sub_mesh.value(), transform);
        }
    }
    return true;
}

void GeometryRenderQueue::sortRenderData() {
    std::sort(
        m_render_data.begin(),
        m_render_data.end(),
        [](GeometryRenderData const& r1, GeometryRenderData const& r2) {
            if(r1.material->getRenderer() != r2.material->getRenderer())
                return r1.material->getRenderer() < r2.material->getRenderer();

            return (r1.mesh != r2.mesh) ? (r1.mesh < r2.mesh) : (r1.material < r2.material);
        }
    );
}

void GeometryRenderQueue::clearRendarData() {
    m_render_data.clear();
}

} // namespace bembel::graphics
