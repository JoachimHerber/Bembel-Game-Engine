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
namespace bembel{

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
		std::string subMesh;
	};

	AssetHandle                         GetMesh();
	const std::vector<MaterialMapping>& GetMateialMapping();

	const static std::string& GetTypeName();

	static std::unique_ptr<GeometryModel> CreateAsset(AssetManager* assetMgr, const xml::Element* properties);
	static std::unique_ptr<GeometryModel> LoadFromFile(AssetManager* assetMgr, const std::string& fileName);

private:

	AssetHandle                  _mesh;
	std::vector<MaterialMapping> _materialMapping;
};

using GeometryModelContainer    = AssetContainer<GeometryModel>;
using GeometryModelContainerPtr = std::shared_ptr<GeometryModelContainer>;


} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
