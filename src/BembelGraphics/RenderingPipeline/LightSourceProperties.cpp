/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "LightSourceProperties.h"

#include <BembelKernel/Scene/PositionComponent.h>

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
	const xml::Element* properties)
{
	component.bulbRadius = 1;
	xml::GetAttribute(properties, "color", component.color);
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
	const xml::Element* properties)
{
	xml::GetAttribute(properties, "color", component.color);
	xml::GetAttribute(properties, "direction", component.direction);
	component.direction = glm::normalize(component.direction);
	return true;
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
