/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "rotation-component.h"

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel {

const std::string& RotationComponent::GetComponentTypeName()
{
	const static std::string type_name = "Rotation";
	return type_name;
}

bool RotationComponent::InitComponent(
	AssetManager*,
	const xml::Element* properties,
	RotationComponent* component)
{
	glm::vec3 axis;
	float angle;
	if( xml::GetAttribute(properties, "axis", axis) &&
		xml::GetAttribute(properties, "angle", angle) )
	{
		angle = glm::radians(angle);
		component->rotation = glm::angleAxis(angle, axis);
	}
	return true;
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
