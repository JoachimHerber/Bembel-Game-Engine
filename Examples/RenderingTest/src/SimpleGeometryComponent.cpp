/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "SimpleGeometryComponent.h"

#include <BembelKernel/Scene/PositionComponent.h>

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel{

const std::string& SimpleGeometryComponent::GetComponentTypeName()
{
	const static std::string typeName = "SimpleGeometry";
	return typeName;
}

bool SimpleGeometryComponent::InitComponent(
	SimpleGeometryComponent& component,
	const xml::Element* properties)
{
	component.size = glm::vec3(1, 1, 1);
	component.albedo = glm::vec4(1, 0, 1, 1);
	component.roughness = 0.5;

	std::string shape;
	xml::GetAttribute(properties, "shape", shape);
	xml::GetAttribute(properties, "size", component.size);
	xml::GetAttribute(properties, "albedo", component.albedo);
	xml::GetAttribute(properties, "roughness", component.roughness);

	if (shape == "SPHERE")
		component.shape = SPHERE;
	else if (shape == "XZ_PLAIN")
		component.shape = XZ_PLAIN;
	else
		return false;

	return true;
}

Entity CreateSphere(
	Scene* world,
	const glm::vec3& positon,
	float radius,
	const glm::vec4& albedo,
	float roughness)
{
 	Entity entity = Entity::CreateEntity(world);

	entity.CreateComponent<PositionComponent>()->position = positon;
	SimpleGeometryComponent* geomComponent =
		entity.CreateComponent<SimpleGeometryComponent>();

	geomComponent->shape = SimpleGeometryComponent::SPHERE;
	geomComponent->size = glm::vec3(2*radius);
	geomComponent->albedo = albedo;
	geomComponent->roughness = roughness;

 	return entity;
}

Entity CreateXZPlain(
	Scene* world,
	const glm::vec3& positon,
	const glm::vec2& size,
	const glm::vec4& albedo,
	float roughness)
{
	Entity entity = Entity::CreateEntity(world);

	entity.CreateComponent<PositionComponent>()->position = positon;
	SimpleGeometryComponent* geomComponent =
		entity.CreateComponent<SimpleGeometryComponent>();

	geomComponent->shape = SimpleGeometryComponent::XZ_PLAIN;
	geomComponent->size = glm::vec3(size.x, 1, size.y);
	geomComponent->albedo = albedo;
	geomComponent->roughness = roughness;

	return entity;
}

Entity CreateBox(
	Scene* world,
	const glm::vec3& positon,
	const glm::vec3& size, 
	const glm::vec4& albedo,
	float roughness)
{
	Entity entity = Entity::CreateEntity(world);

	entity.CreateComponent<PositionComponent>()->position = positon;
	SimpleGeometryComponent* geomComponent =
		entity.CreateComponent<SimpleGeometryComponent>();

	geomComponent->shape = SimpleGeometryComponent::BOX;
	geomComponent->size = size;
	geomComponent->albedo = albedo;
	geomComponent->roughness = roughness;

	return entity;
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
