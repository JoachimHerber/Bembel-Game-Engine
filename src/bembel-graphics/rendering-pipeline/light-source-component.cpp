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
	AssetManager*,
	const xml::Element* properties,
	PointLightProperties* component	)
{
	component->bulb_radius = 1;
	xml::GetAttribute(properties, "color", component->color);
	float intensity;
	if( xml::GetAttribute(properties, "intensity", intensity) )
		component->color *= intensity;

	xml::GetAttribute(properties, "bulbRadius", component->bulb_radius);
	xml::GetAttribute(properties, "cutoffRadius", component->cutoff_radius);

	return true;
}

const std::string& DirLightProperties::GetComponentTypeName()
{
	const static std::string typeName = "DirectionalLight";
	return typeName;
}

bool DirLightProperties::InitComponent(
	AssetManager*,
	const xml::Element* properties,
	DirLightProperties* component)
{
	xml::GetAttribute(properties, "color", component->color);
	float intensity;
	if( xml::GetAttribute(properties, "intensity", intensity) )
		component->color *= intensity;
	xml::GetAttribute(properties, "direction", component->direction);
	component->direction = glm::normalize(component->direction);

	return true;
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
