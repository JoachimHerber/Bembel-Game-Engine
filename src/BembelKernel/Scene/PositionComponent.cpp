/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "PositionComponent.h"

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel {

const std::string& PositionComponent::GetComponentTypeName()
{
	const static std::string typeName = "Position";
	return typeName;
}

bool PositionComponent::InitComponent(
	PositionComponent& component, 
	const xml::Element* properties)
{
	xml::GetAttribute(properties, "x", component.position.x);
	xml::GetAttribute(properties, "y", component.position.y);
	xml::GetAttribute(properties, "z", component.position.z);
	return true;
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
