#ifndef BEMBEL_GRAPHICS_GEOMETRY_GEOMETRYINSTANCE_H_
#define BEMBEL_GRAPHICS_GEOMETRY_GEOMETRYINSTANCE_H_
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "bembel-config.h"
#include "bembel-kernel/assets/asset-handle.h"
#include "bembel-kernel/scene/scene.h"
#include "bembel-kernel/scene/entity.h"
#include "bembel-kernel/scene/component-container.hpp"

#include <memory>

#include <glm/glm.hpp>

/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel {

class BEMBEL_API GeometryComponent
{
public:
	AssetHandle model;

	// the ID of the renderer that should be used for rendering the geometry
	unsigned    renderer;

	using ContainerType = DenseComponentContainer<GeometryComponent>;
	using ContainerPtr  = std::shared_ptr<ContainerType>;

	static const std::string& GetComponentTypeName();
	static bool InitComponent(
		GeometryComponent&,
		const xml::Element*,
		AssetManager*);
};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards

