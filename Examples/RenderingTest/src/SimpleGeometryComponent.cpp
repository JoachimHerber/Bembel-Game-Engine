/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "SimpleGeometryComponent.h"

#include <BembelKernel/Scene/PositionComponent.h>
#include <BembelKernel/Assets/AssetManager.h>
#include <BembelGraphics/OpenGL/Texture.h>

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel{

const std::string& SimpleGeometryComponent::GetComponentTypeName()
{
	const static std::string typeName = "SimpleGeometry";
	return typeName;
}

bool SimpleGeometryComponent::InitComponent(
	SimpleGeometryComponent& component,
	const xml::Element* properties, 
	AssetManager* assetMgr)
{
	component.size = glm::vec3(1, 1, 1);
	component.roughness = 0.5;

	std::string shape;
	std::string albedo;
	xml::GetAttribute(properties, "shape", shape);
	xml::GetAttribute(properties, "size", component.size);
	xml::GetAttribute(properties, "albedo", albedo);
	xml::GetAttribute(properties, "roughness", component.roughness);

	component.albedo = assetMgr->GetAssetHandle<Texture>(albedo);

	if (shape == "SPHERE")
		component.shape = SPHERE;
	else if (shape == "XZ_PLAIN")
		component.shape = XZ_PLAIN;
	else
		return false;

	return true;
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
