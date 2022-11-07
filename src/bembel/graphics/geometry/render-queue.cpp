module;
#include <algorithm>

#include "bembel/pch.h"
module bembel.graphics.geometry;

import bembel.base;
import bembel.kernel;

namespace bembel::graphics {
using namespace bembel::base;
using namespace bembel::kernel;

GeometryRenderQueue::GeometryRenderQueue(AssetManager& asset_mgr) : m_asset_mgr{asset_mgr} {}

GeometryRenderQueue::~GeometryRenderQueue() {}

bool GeometryRenderQueue::addGeometryObject(AssetHandle modelHndl, mat4 const& transform) {
    auto* model = m_asset_mgr.getAsset<GeometryModel>(modelHndl);
    return addGeometryObject(model, transform);
}

bool GeometryRenderQueue::addGeometryObject(GeometryModel* model, mat4 const& transform) {
    if(model == nullptr) return false;

    auto mesh = model->getMesh();
    if(mesh == nullptr) return false;

    for(auto const& mat_map : model->getMateialMapping()) {
        Material* mat = mat_map.material.getAsset();
        if(mesh == nullptr) continue;

        unsigned first, count;
        if(mesh->getSubMesh(mat_map.sub_mesh, first, count)) {
            m_render_data.push_back({mesh, mat, first, count, transform});
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
