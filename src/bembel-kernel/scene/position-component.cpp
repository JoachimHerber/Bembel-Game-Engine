/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "position-component.h"

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel {

const std::string& PositionComponent::GetComponentTypeName()
{
	const static std::string type_name = "Position";
	return type_name;
}

bool PositionComponent::InitComponent(
	AssetManager*,
	const xml::Element* properties,
	PositionComponent* component)
{
	xml::GetAttribute(properties, "x", component->position.x);
	xml::GetAttribute(properties, "y", component->position.y);
	xml::GetAttribute(properties, "z", component->position.z);
	return true;
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
