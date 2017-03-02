#ifndef BEMBEL_GEOMETRYINSTANCE_H
#define BEMBEL_GEOMETRYINSTANCE_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include <BembelConfig.h>
#include <BembelKernel/Scene/Scene.h>
#include <BembelKernel/Scene/Entity.h>
#include <BembelKernel/Scene/ComponentContainer.hpp>
#include <BembelKernel/Assets/AssetHandle.h>

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

