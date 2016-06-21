#ifndef BEMBEL_COMPONENCONTAINER_H
#define BEMBEL_COMPONENCONTAINER_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include <BembelConfig.h>

#include "EntityManager.h"

#include <BembelBase/XML.h>

#include <vector>
#include <map>

/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel {

class BEMBEL_API ComponentContainerBase
{
public:
	ComponentContainerBase(
		EntityManager::ComponentTypeID
	);
	virtual ~ComponentContainerBase(){}

	virtual bool CreateComponent(EntityManager::EntityID, const xml::Element*) = 0;

	EntityManager::ComponentTypeID GetComponentTypeID();
	EntityManager::ComponentMask   GetComponentMask();

private:
	EntityManager::ComponentTypeID _typeID;
	EntityManager::ComponentMask   _mask;
};

template<class ComponentType>
class SparseComponentContainer : public ComponentContainerBase
{
public:
	SparseComponentContainer(
		EntityManager::ComponentTypeID id)
		: ComponentContainerBase(id)
	{}
	virtual ~SparseComponentContainer() {}

	ComponentType* CreateComponent(EntityManager::EntityID);
	bool CreateComponent(EntityManager::EntityID, const xml::Element*) override;

	std::map<EntityManager::EntityID, ComponentType>& GetComponents();
	ComponentType* GetComponent(EntityManager::EntityID);

private:
	std::map<EntityManager::EntityID, ComponentType> _components;
};

template<class ComponentType>
class DenseComponentContainer : public ComponentContainerBase
{
public:
	DenseComponentContainer(
		EntityManager::ComponentTypeID id)
		: ComponentContainerBase(id)
	{}
	virtual ~DenseComponentContainer() {}

	ComponentType* CreateComponent(EntityManager::EntityID);
	bool CreateComponent(EntityManager::EntityID, const xml::Element*) override;

	std::vector<ComponentType>& GetComponents();
	ComponentType* GetComponent(EntityManager::EntityID);

private:
	std::vector<ComponentType> _components;
};
} //end of namespace bembel
/*============================================================================*/
/* INLINE IMPLEMENTATIONS													  */
/*============================================================================*/

#include "ComponentContainer.inl"

/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
