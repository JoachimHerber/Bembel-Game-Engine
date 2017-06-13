#ifndef BEMBEL_SELECTIONSHAPE_H
#define BEMBEL_SELECTIONSHAPE_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include <bembel-base/xml.h>
#include <bembel-kernel/scene/component-container.hpp>

#include <memory>

/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
struct SelectionComponent
{
	enum
	{
		UNSELECTABLE,
		SELECTABLE,
		FOCUSED,
		SELECTED
	} state;


	using ContainerType = bembel::ComponentArray<SelectionComponent>;
	using ContainerPtr = std::shared_ptr<ContainerType>;

	static const std::string& GetComponentTypeName();
	static bool InitComponent(
		bembel::AssetManager*,
		const bembel::xml::Element*,
		SelectionComponent*);
};
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
