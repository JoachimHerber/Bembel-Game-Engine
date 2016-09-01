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

class BEMBEL_API GeometryInstance
{
public:
	AssetHandle model;
	bool isCastingShadow;

	using ContainerType = DenseComponentContainer<GeometryInstance>;
	using ContainerPtr  = std::shared_ptr<ContainerType>;

	static const std::string& GetComponentTypeName();
	static bool InitComponent(
		GeometryInstance&,
		const xml::Element*,
		AssetManager*);
};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards

