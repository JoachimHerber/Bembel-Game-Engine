#ifndef BEMBEL_GEOMETRYMODEL_H
#define BEMBEL_GEOMETRYMODEL_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include <BembelOpenGL.h>
#include <BembelConfig.h>

#include <BembelKernel/Assets/AssetHandle.h>
#include <BembelKernel/Assets/AssetContainer.h>
#include <BembelKernel/Assets/SerialAssetLoader.hpp>

#include "Material.h"

#include <memory>
#include <vector>

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
