/******************************************************************************/
/* ************************************************************************** */
/* *                                                                        * */
/* *    MIT License                                                         * */
/* *                                                                        * */
/* *   Copyright(c) 2017 Joachim Herber                                     * */
/* *                                                                        * */
/* *   Permission is hereby granted, free of charge, to any person          * */
/* *   obtaining copy of this software and associated documentation files   * */
/* *   (the "Software"), to deal in the Software without restriction,       * */
/* *   including without limitation the rights to use, copy, modify, merge, * */
/* *   publish, distribute, sublicense, and/or sell copies of the Software, * */
/* *   and to permit persons to whom the Software is furnished to do so,    * */
/* *   subject to the following conditions :                                * */
/* *                                                                        * */
/* *   The above copyright notice and this permission notice shall be       * */
/* *   included in all copies or substantial portions of the Software.      * */
/* *                                                                        * */
/* *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,      * */
/* *   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF   * */
/* *   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND                * */
/* *   NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS   * */
/* *   BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN   * */
/* *   ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN    * */
/* *   CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE     * */
/* *   SOFTWARE.                                                            * */
/* *                                                                        * */
/* ************************************************************************** */
/******************************************************************************/

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
