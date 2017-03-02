/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "light-source-component.h"

#include <bembel-kernel/scene/position-component.h>

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel {

const std::string& PointLightProperties::GetComponentTypeName()
{
	const static std::string typeName = "PointLight";
	return typeName;
}

bool PointLightProperties::InitComponent(
	PointLightProperties& component, 
	const xml::Element* properties, 
	AssetManager*)
{
	component.bulbRadius = 1;
	xml::GetAttribute(properties, "color", component.color);
	float intensity;
	if (xml::GetAttribute(properties, "intensity", intensity))
		component.color *= intensity;

	xml::GetAttribute(properties, "bulbRadius",   component.bulbRadius);
	xml::GetAttribute(properties, "cutoffRadius", component.cutoffRadius);

	return true;
}

const std::string& DirLightProperties::GetComponentTypeName()
{
	const static std::string typeName = "DirectionalLight";
	return typeName;
}

bool DirLightProperties::InitComponent(
	DirLightProperties& component,
	const xml::Element* properties, 
	AssetManager*)
{
	xml::GetAttribute(properties, "color", component.color); 
	float intensity;
	if (xml::GetAttribute(properties, "intensity", intensity))
		component.color *= intensity;
	xml::GetAttribute(properties, "direction", component.direction);
	component.direction = glm::normalize(component.direction);
	
	return true;
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
