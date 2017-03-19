/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "geometry-component.h"
#include "geometry-model.h"

#include "bembel-kernel/assets/asset-manager.h"

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel {

const std::string& GeometryComponent::GetComponentTypeName()
{
	const static std::string type_name = "GeometryComponent";
	return type_name;
}

bool GeometryComponent::InitComponent(
	AssetManager* asset_manager,
	const xml::Element* properties,
	GeometryComponent* geometry_component )
{
	std::string model_name;
	if( !xml::GetAttribute(properties, "model", model_name) )
		return false;

	geometry_component->model = 
		asset_manager->GetAssetHandle<GeometryModel>(model_name);

	return true;
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
