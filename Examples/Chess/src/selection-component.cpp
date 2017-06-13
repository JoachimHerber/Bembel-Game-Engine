/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "selection-component.h"

#include <limits>

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
const std::string& SelectionComponent::GetComponentTypeName()
{
	const static std::string typeName = "SelectionComponent";
	return typeName;
}

bool SelectionComponent::InitComponent(
	bembel::AssetManager*,
	const bembel::xml::Element* elem,
	SelectionComponent* comp)
{
	bool b;
	if ( bembel::xml::GetAttribute(elem, "selectable", b) && b)
		comp->state = SELECTABLE;
	else
		comp->state = UNSELECTABLE;

	return true;
}
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
