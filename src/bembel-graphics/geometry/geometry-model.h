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
	static std::unique_ptr<GeometryModel> LoadAsset(
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
