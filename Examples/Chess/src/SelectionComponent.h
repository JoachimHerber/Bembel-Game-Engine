#ifndef BEMBEL_SELECTIONSHAPE_H
#define BEMBEL_SELECTIONSHAPE_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include <BembelBase/XML.h>
#include <BembelKernel/Scene/ComponentContainer.hpp>

#include <memory>

/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel {

struct SelectionComponent
{
	enum
	{
		UNSELECTABLE,
		SELECTABLE,
		FOCUSED,
		SELECTED
	} state;


	using ContainerType = DenseComponentContainer<SelectionComponent>;
	using ContainerPtr = std::shared_ptr<ContainerType>;

	static const std::string& GetComponentTypeName();
	static bool InitComponent(
		SelectionComponent&,
		const xml::Element*,
		AssetManager*);
};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
