#ifndef BEMBEL_GRAPHICS_RENDERING_PIPELINE_LIGHTSOURCE_H_
#define BEMBEL_GRAPHICS_RENDERING_PIPELINE_LIGHTSOURCE_H_
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "bembel-config.h"

#include <memory>
#include <glm/glm.hpp>

#include <bembel-kernel/scene/scene.h>
#include <bembel-kernel/scene/entity.h>
#include <bembel-kernel/scene/component-container.hpp>

/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel {

class BEMBEL_API PointLightProperties
{
public:
	glm::vec3 color;

	float bulbRadius;
	float cutoffRadius;

	using ContainerType = SparseComponentContainer<PointLightProperties>;
	using ContainerPtr  = std::shared_ptr<ContainerType>;

	static const std::string& GetComponentTypeName();
	static bool InitComponent(
		PointLightProperties&,
		const xml::Element*,
		AssetManager*);
};

class BEMBEL_API DirLightProperties
{
public:
	glm::vec3 color;
	glm::vec3 direction;

	using ContainerType = SparseComponentContainer<DirLightProperties>;
	using ContainerPtr = std::shared_ptr<ContainerType>;

	static const std::string& GetComponentTypeName();
	static bool InitComponent(
		DirLightProperties&, 
		const xml::Element*, 
		AssetManager*);
};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards

