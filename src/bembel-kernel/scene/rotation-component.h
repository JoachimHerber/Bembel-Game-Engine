#ifndef BEMBEL_KERNEL_SCENE_ROTATION_COMPONENT_H_
#define BEMBEL_KERNEL_SCENE_ROTATION_COMPONENT_H_
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "bembel-config.h"
#include "scene.h"
#include "component-container.hpp"

#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel {

struct BEMBEL_API RotationComponent
{
	glm::quat rotation;

	using ContainerType = DenseComponentContainer<RotationComponent>;

	static const std::string& GetComponentTypeName();
	static bool InitComponent(
		AssetManager*,
		const xml::Element*,
		RotationComponent* );
};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
