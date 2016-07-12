#ifndef BEMBEL_SIMPLEGEOMETRYCOMPONENT_H
#define BEMBEL_SIMPLEGEOMETRYCOMPONENT_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include <BembelConfig.h>

#include <BembelKernel/Assets/AssetHandle.h>
#include <BembelKernel/Scene/Scene.h>
#include <BembelKernel/Scene/Entity.h>
#include <BembelKernel/Scene/ComponentContainer.hpp>

#include <glm/glm.hpp>

/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel{

struct SimpleGeometryComponent
{
	enum Shape
	{
		SPHERE = 0,
		XZ_PLAIN,
		BOX,
		NUM_SHAPES
	};

	Shape shape;

	glm::vec3 size;

	AssetHandle albedo;
	float     roughness;

	using ContainerType = DenseComponentContainer<SimpleGeometryComponent>;
	using ContainerPtr = std::shared_ptr<ContainerType>;

	static const std::string& GetComponentTypeName();
	static bool InitComponent(SimpleGeometryComponent&, const xml::Element*, AssetManager*);
};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
