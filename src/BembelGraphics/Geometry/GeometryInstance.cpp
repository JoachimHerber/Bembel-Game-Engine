/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "GeometryInstance.h"
#include "GeometryModel.h"

#include <BembelKernel/Assets/AssetManager.h>

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel {

const std::string& GeometryInstance::GetComponentTypeName()
{
	const static std::string typeName = "GeometryInstance";
	return typeName;
}

bool GeometryInstance::InitComponent(
	GeometryInstance& geom,
	const xml::Element* properties, 
	AssetManager* assetMgr)
{
	std::string modelName;
	if (!xml::GetAttribute(properties, "model", modelName))
		return false;

	geom.model = assetMgr->GetAssetHandle<GeometryModel>(modelName);

	return true;
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
