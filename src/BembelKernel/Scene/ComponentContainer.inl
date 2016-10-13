/*============================================================================*/
/* INLINE IMPLEMENTETION                                                      */
/*============================================================================*/
namespace bembel{

inline ComponentContainerBase::ComponentContainerBase(
	Scene::ComponentTypeID id)
	: _typeID(id)
	, _mask(1<<id)
{}

inline Scene::ComponentTypeID ComponentContainerBase::GetComponentTypeID()
{
	return _typeID;
}

inline Scene::ComponentTypeID ComponentContainerBase::GetComponentMask()
{
	return _mask;
}

template<class ComponentType>
inline ComponentType* SparseComponentContainer<ComponentType>::CreateComponent(
	Scene::EntityID entity)
{
	return &_components[entity];
}

template<class ComponentType>
inline bool SparseComponentContainer<ComponentType>::CreateComponent(
	Scene::EntityID entity, 
	const xml::Element* properties, 
	AssetManager* assetMgr)
{
	ComponentType component;
	if(ComponentType::InitComponent(component, properties, assetMgr))
	{
		_components[entity] = component;
		return true;
	}
	return false;
}

template<class ComponentType>
inline std::map<Scene::EntityID, ComponentType>& 
	SparseComponentContainer<ComponentType>::GetComponents()
{
	return _components;
}

template<class ComponentType>
inline ComponentType* SparseComponentContainer<ComponentType>::GetComponent(
	Scene::EntityID entity)
{
	auto it = _components.find(entity);
	if (it != _components.end())
		return &(it->second);

	return nullptr;
}

template<class ComponentType>
inline ComponentType* DenseComponentContainer<ComponentType>::CreateComponent(
	Scene::EntityID entity)
{
	if (entity >= _components.size())
		_components.resize(entity + 1);

	return &_components[entity];
}
template<class ComponentType>
inline bool DenseComponentContainer<ComponentType>::CreateComponent(
	Scene::EntityID entity,
	const xml::Element* properties, 
	AssetManager* assetMgr)
{
	ComponentType component;
	if(ComponentType::InitComponent(component, properties, assetMgr))
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
	Scene::EntityID entity)
{
	assert(entity < _components.size());
	return &_components[entity];
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/