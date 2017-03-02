/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "entity.h"

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel {

Entity::Entity(Scene* world, Scene::EntityID id)
	: _world(world)
	, _id(id)
{}
Entity::Entity(const Entity& other)
	: _world(other._world)
	, _id(other._id)
{}
Entity::Entity(Entity&& other)
	: _world(other._world)
	, _id(other._id)
{}

Entity  Entity::CreateEntity(Scene* world)
{
	return Entity(world, world->CreateEntity());
}


} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
