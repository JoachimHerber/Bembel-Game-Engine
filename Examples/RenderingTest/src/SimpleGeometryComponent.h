#ifndef BEMBEL_SIMPLEGEOMETRYCOMPONENT_H
#define BEMBEL_SIMPLEGEOMETRYCOMPONENT_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include <BembelConfig.h>

#include <BembelDataStructures/EntityManager.h>
#include <BembelDataStructures/Entity.h>
#include <BembelDataStructures/ComponentContainer.hpp>

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

	glm::vec4 albedo;
	float     roughness;

	using ContainerType = DenseComponentContainer<SimpleGeometryComponent>;
	using ContainerPtr = std::shared_ptr<ContainerType>;

	static const std::string& GetComponentTypeName();
	static bool InitComponent(SimpleGeometryComponent&, const xml::Element*);
};

Entity CreateSphere(
	EntityManager* world,
	const glm::vec3& positon,
	float radius,
	const glm::vec4& albedo,
	float roughness);

Entity CreateXZPlain(
	EntityManager* world,
	const glm::vec3& positon,
	const glm::vec2& size,
	const glm::vec4& albedo,
	float roughness);

Entity CreateBox(
	EntityManager* world,
	const glm::vec3& positon,
	const glm::vec3& size,
	const glm::vec4& albedo,
	float roughness);

} //end of namespace bembel
  /*============================================================================*/
  /* END OF FILE                                                                */
  /*============================================================================*/
#endif //include guards