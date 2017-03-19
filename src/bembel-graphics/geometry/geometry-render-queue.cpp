/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "geometry-render-queue.h"
#include "geometry-model.h"
#include "geometry-mesh.h"

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel {

GeometryRenderQueue::GeometryRenderQueue(AssetManager* asset_manager)
	: asset_manager_(asset_manager)
{}

GeometryRenderQueue::~GeometryRenderQueue()
{}

bool GeometryRenderQueue::AddGeometryObject(
	AssetHandle modelHndl,
	const glm::mat4& transform)
{
	auto* model = asset_manager_->GetAsset<GeometryModel>(modelHndl);
	return AddGeometryObject(model, transform);
}

bool GeometryRenderQueue::AddGeometryObject(
	GeometryModel* model,
	const glm::mat4& transform)
{
	if( model == nullptr )
		return false;

	auto mesh = asset_manager_->GetAsset<GeometryMesh>(model->GetMesh());
	if( mesh == nullptr )
		return false;

	for( const auto& mat_map : model->GetMateialMapping() )
	{
		Material* mat = asset_manager_->GetAsset<Material>(mat_map.material);
		if( mesh == nullptr )
			continue;

		unsigned first, count;
		if( mesh->GetSubMesh(mat_map.sub_mesh, first, count) )
		{
			render_data_.push_back({
				mesh, mat, first, count, transform});
		}
	}
	return true;
}

void GeometryRenderQueue::SortRenderData()
{
	std::sort(render_data_.begin(), render_data_.end(), [](
		const GeometryRenderData& r1,
		const GeometryRenderData& r2
		)
	{
		if( r1.material->GetRenderer() != r2.material->GetRenderer() )
			return r1.material->GetRenderer() < r2.material->GetRenderer();

		return
			(r1.mesh != r2.mesh) ?
			(r1.mesh < r2.mesh) :
			(r1.material < r2.material);
	});
}

void GeometryRenderQueue::ClearRendarData()
{
	render_data_.clear();
}

const std::vector<GeometryRenderData>& GeometryRenderQueue::GetRenderData() const
{
	return render_data_;
}


} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
