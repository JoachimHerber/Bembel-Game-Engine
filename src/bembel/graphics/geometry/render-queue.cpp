#include "./render-queue.hpp"

#include "./mesh.hpp"
#include "./model.hpp"

namespace bembel::graphics {

GeometryRenderQueue::GeometryRenderQueue(kernel::AssetManager& asset_mgr)
: asset_mgr{asset_mgr} {
}

GeometryRenderQueue::~GeometryRenderQueue() {
}

bool GeometryRenderQueue::addGeometryObject(
    kernel::AssetHandle modelHndl, const glm::mat4& transform) {
    auto* model = this->asset_mgr.getAsset<GeometryModel>(modelHndl);
    return addGeometryObject(model, transform);
}

bool GeometryRenderQueue::addGeometryObject(GeometryModel* model, const glm::mat4& transform) {
    if(model == nullptr) return false;

    auto mesh = model->getMesh();
    if(mesh == nullptr) return false;

    for(const auto& mat_map : model->getMateialMapping()) {
        Material* mat = mat_map.material.get();
        if(mesh == nullptr) continue;

        unsigned first, count;
        if(mesh->getSubMesh(mat_map.sub_mesh, first, count)) {
            this->render_data.push_back({mesh, mat, first, count, transform});
        }
    }
    return true;
}

void GeometryRenderQueue::sortRenderData() {
    std::sort(
        this->render_data.begin(),
        this->render_data.end(),
        [](const GeometryRenderData& r1, const GeometryRenderData& r2) {
            if(r1.material->getRenderer() != r2.material->getRenderer())
                return r1.material->getRenderer() < r2.material->getRenderer();

            return (r1.mesh != r2.mesh) ? (r1.mesh < r2.mesh) : (r1.material < r2.material);
        });
}

void GeometryRenderQueue::clearRendarData() {
    this->render_data.clear();
}

} // namespace bembel::graphics
