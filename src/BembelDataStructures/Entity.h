#ifndef BEMBEL_ENTITY_H
#define BEMBEL_ENTITY_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include <BembelConfig.h>
#include "EntityManager.h"

/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel {

class BEMBEL_API Entity
{
public:
	Entity(EntityManager*, EntityManager::EntityID);
	Entity(const Entity&);
	Entity(Entity&&);

	template<class ComponentType>
	bool HasComponent();

	template<class ComponentType>
	ComponentType* CreateComponent();

	template<class ComponentType>
	ComponentType* GetComponent();

	static Entity CreateEntity(EntityManager*);

private:
	EntityManager*          _world;
	EntityManager::EntityID _id;
};

} //end of namespace bembel
/*============================================================================*/
/* INLINE IMPLEMENTATIONS													  */
/*============================================================================*/
namespace bembel{

template<class ComponentType>
inline bool Entity::HasComponent()
{
	if (!_world)
		return false;

	return _world->GetEntitys()[_id] & ComponentType::GetComponentMask() != 0;
}

template<class ComponentType>
inline ComponentType* Entity::CreateComponent()
{
	if (!_world)
		return nullptr;

	return _world->CreateComponent<ComponentType>(_id);
}

template<class ComponentType>
inline ComponentType* Entity::GetComponent()
{
	if (!_world)
		return nullptr;

	if (_world->GetEntitys()[_id] & ComponentType::GetComponentMask() != 0)
		return nullptr;

	auto* container = _world->GetComponentContainer<ComponentType>();
	return container->GetComponent(_id);
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
