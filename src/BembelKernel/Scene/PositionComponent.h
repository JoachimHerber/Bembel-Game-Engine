#ifndef BEMBEL_POSITIONCOMPONENT_H
#define BEMBEL_POSITIONCOMPONENT_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include <BembelConfig.h>

#include "EntityManager.h"
#include "ComponentContainer.hpp"

#include <memory>
#include <glm/glm.hpp>

/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel {

struct BEMBEL_API PositionComponent
{
	glm::vec3 position;

	using ContainerType = DenseComponentContainer<PositionComponent>;
	using ContainerPtr = std::shared_ptr<ContainerType>;

	static const std::string& GetComponentTypeName();
	static bool InitComponent(PositionComponent&, const xml::Element*);
};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
