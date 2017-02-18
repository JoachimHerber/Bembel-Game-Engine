/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "GeometryRenderQueue.h"

#include "GeometryModel.h"
#include "GeometryMesh.h"


/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel {

GeometryRenderQueue::GeometryRenderQueue()
{}

GeometryRenderQueue::~GeometryRenderQueue()
{}

void GeometryRenderQueue::SetAssetMannager(AssetManager* assetMgr)
{
	_assetMgr = assetMgr;
}

bool GeometryRenderQueue::AddGeometryObject(
	AssetHandle modelHndl, 
	const glm::mat4& transform)
{
	auto* model = _assetMgr->GetAsset<GeometryModel>(modelHndl);
	return AddGeometryObject(model, transform);
}

bool GeometryRenderQueue::AddGeometryObject(
	GeometryModel* model, 
	const glm::mat4& transform)
{
	if (model == nullptr)
		return false;

	auto mesh = _assetMgr->GetAsset<GeometryMesh>(model->GetMesh());
	if (mesh == nullptr)
		return false;

	for (const auto& matMap : model->GetMateialMapping())
	{
		Material* mat = _assetMgr->GetAsset<Material>(matMap.material);
		if (mesh == nullptr)
			continue;

		unsigned first, count;
		if (mesh->GetSubMesh(matMap.subMesh, first, count))
		{
			_renderData.push_back({
				mesh, mat, first, count, transform });
		}
	}
	return true;
}

void GeometryRenderQueue::SortRenderData()
{
	std::sort(_renderData.begin(), _renderData.end(), [](
			const GeometryRenderData& r1,
			const GeometryRenderData& r2
	){
		if (r1.material->GetRenderer() != r2.material->GetRenderer())
			return r1.material->GetRenderer() < r2.material->GetRenderer();
		
		return
			(r1.mesh != r2.mesh) ?
			(r1.mesh < r2.mesh) :
			(r1.material < r2.material);
	});
}

void GeometryRenderQueue::ClearRendarData()
{
	_renderData.clear();
}

const std::vector<GeometryRenderData>& GeometryRenderQueue::GetRenderData() const
{
	return _renderData;
}


} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
