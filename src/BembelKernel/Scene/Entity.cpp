/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "Entity.h"

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel {

Entity::Entity(EntityManager* world, EntityManager::EntityID id)
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

Entity  Entity::CreateEntity(EntityManager* world)
{
	return Entity(world, world->CreateEntity());
}


} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
