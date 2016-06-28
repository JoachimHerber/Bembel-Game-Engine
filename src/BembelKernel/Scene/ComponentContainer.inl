/*============================================================================*/
/* INLINE IMPLEMENTETION                                                      */
/*============================================================================*/
namespace bembel{

inline ComponentContainerBase::ComponentContainerBase(
	EntityManager::ComponentTypeID id)
	: _typeID(id)
	, _mask(1<<id)
{}

inline EntityManager::ComponentTypeID ComponentContainerBase::GetComponentTypeID()
{
	return _typeID;
}

inline EntityManager::ComponentTypeID ComponentContainerBase::GetComponentMask()
{
	return _mask;
}

template<class ComponentType>
inline ComponentType* SparseComponentContainer<ComponentType>::CreateComponent(
	EntityManager::EntityID entity)
{
	return &_components[entity];
}

template<class ComponentType>
inline bool SparseComponentContainer<ComponentType>::CreateComponent(
	EntityManager::EntityID entity, 
	const xml::Element* properties)
{
	ComponentType component;
	if(ComponentType::InitComponent(component, properties))
	{
		_components[entity] = component;
		return true;
	}
	return false;
}

template<class ComponentType>
inline std::map<EntityManager::EntityID, ComponentType>& 
	SparseComponentContainer<ComponentType>::GetComponents()
{
	return _components;
}

template<class ComponentType>
inline ComponentType* SparseComponentContainer<ComponentType>::GetComponent(
	EntityManager::EntityID entity)
{
	auto it = _components.find(entity);
	if (it != _components.end())
		return &(it->second);

	return nullptr;
}

template<class ComponentType>
inline ComponentType* DenseComponentContainer<ComponentType>::CreateComponent(
	EntityManager::EntityID entity)
{
	if (entity >= _components.size())
		_components.resize(entity + 1);

	return &_components[entity];
}
template<class ComponentType>
inline bool DenseComponentContainer<ComponentType>::CreateComponent(
	EntityManager::EntityID entity,
	const xml::Element* properties)
{
	ComponentType component;
	if(ComponentType::InitComponent(component, properties))
	{
		if (entity >= _components.size())
			_components.resize(entity + 1);

		_components[entity] = component;
		return true;
	}
	return false;
}

template<class ComponentType>
inline std::vector<ComponentType>&
	DenseComponentContainer<ComponentType>::GetComponents()
{
	return _components;
}

template<class ComponentType>
inline ComponentType* DenseComponentContainer<ComponentType>::GetComponent(
	EntityManager::EntityID entity)
{
	if (entity >= _components.size())
		return nullptr;

	return &_components[entity];
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/