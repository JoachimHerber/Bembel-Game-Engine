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

#ifndef BEMBEL_GRAPHICS_GEOMETRY_GEOMETRYMODEL_H_
#define BEMBEL_GRAPHICS_GEOMETRY_GEOMETRYMODEL_H_
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "bembel-open-gl.h"
#include "bembel-config.h"

#include "Material.h"

#include <memory>
#include <vector>

#include <bembel-kernel/assets/asset-handle.h>
#include <bembel-kernel/assets/asset-container.h>
#include <bembel-kernel/assets/serial-asset-loader.hpp>

/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel {

class BEMBEL_API GeometryModel
{
public:
	using DefaultLoaderType = SerialAssetLoader<GeometryModel>;

public:
	GeometryModel();
	~GeometryModel();

	struct MaterialMapping
	{
		AssetHandle material;
		std::string sub_mesh;
	};

	AssetHandle                         GetMesh();
	const std::vector<MaterialMapping>& GetMateialMapping();

	const static std::string& GetTypeName();

	static std::unique_ptr<GeometryModel> LoadAsset(
		AssetManager* asset_manager, const std::string& file_name);
	static std::unique_ptr<GeometryModel> CreateAsset(
		AssetManager* asset_manager, const xml::Element* properties);

	static void DeleteAsset(
		AssetManager* asset_manager, 
		std::unique_ptr<GeometryModel> model);

private:
	static std::unique_ptr<GeometryModel> CreateGeometryModel(
		AssetManager* asset_manager, const xml::Element* properties);

private:
	AssetHandle                  mesh_;
	std::vector<MaterialMapping> material_mapping_;
};

using GeometryModelContainer = AssetContainer<GeometryModel>;
using GeometryModelContainerPtr = std::shared_ptr<GeometryModelContainer>;


} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
