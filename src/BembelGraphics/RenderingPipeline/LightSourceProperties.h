#ifndef BEMBEL_LIGHTSOURCE_H
#define BEMBEL_LIGHTSOURCE_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include <BembelConfig.h>
#include <BembelKernel/Scene/EntityManager.h>
#include <BembelKernel/Scene/Entity.h>
#include <BembelKernel/Scene/ComponentContainer.hpp>

#include <memory>
#include <glm/glm.hpp>

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
	static bool InitComponent(PointLightProperties&, const xml::Element*);
};

class BEMBEL_API DirLightProperties
{
public:
	glm::vec3 color;
	glm::vec3 direction;

	using ContainerType = SparseComponentContainer<DirLightProperties>;
	using ContainerPtr = std::shared_ptr<ContainerType>;

	static const std::string& GetComponentTypeName();
	static bool InitComponent(DirLightProperties&, const xml::Element*);
};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards

