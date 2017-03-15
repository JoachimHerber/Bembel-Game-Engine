#ifndef BEMBEL_KERNEL_SCENE_POSITIONCOMPONENT_H_
#define BEMBEL_KERNEL_SCENE_POSITIONCOMPONENT_H_
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "bembel-config.h"
#include "scene.h"
#include "component-container.hpp"

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

	static const std::string& GetComponentTypeName();
	static bool InitComponent(
		PositionComponent&, 
		const xml::Element*, 
		AssetManager*);
};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
