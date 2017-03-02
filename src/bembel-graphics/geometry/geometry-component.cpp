/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "GeometryComponent.h"
#include "GeometryModel.h"

#include <BembelKernel/Assets/AssetManager.h>

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel {

const std::string& GeometryComponent::GetComponentTypeName()
{
	const static std::string typeName = "GeometryComponent";
	return typeName;
}

bool GeometryComponent::InitComponent(
	GeometryComponent& geom,
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
