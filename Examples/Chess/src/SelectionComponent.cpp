/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "SelectionComponent.h"

#include <limits>

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel {

const std::string& SelectionComponent::GetComponentTypeName()
{
	const static std::string typeName = "SelectionComponent";
	return typeName;
}

bool SelectionComponent::InitComponent(
	SelectionComponent& comp,
	const xml::Element* elem,
	AssetManager*)
{
	bool b;
	if (xml::GetAttribute(elem, "selectable", b) && b)
		comp.stat = SELECTABLE;
	else
		comp.stat = UNSELECTABLE;

	return true;
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
