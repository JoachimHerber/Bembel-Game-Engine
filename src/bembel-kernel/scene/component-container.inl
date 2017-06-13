/*============================================================================*/
/* INLINE IMPLEMENTETION                                                      */
/*============================================================================*/
namespace bembel {

inline ComponentContainerBase::ComponentContainerBase(
	Scene::ComponentTypeID id)
	: type_id_(id)
	, mask_(1<<id)
{}

inline Scene::ComponentTypeID ComponentContainerBase::GetComponentTypeID()
{
	return type_id_;
}

inline Scene::ComponentTypeID ComponentContainerBase::GetComponentMask()
{
	return mask_;
}

template<class ComponentType>
inline ComponentType* ComponentMap<ComponentType>::CreateComponent(
	Scene::EntityID entity)
{
	return &components_[entity];
}

template<class ComponentType>
inline bool ComponentMap<ComponentType>::CreateComponent(
	Scene::EntityID entity,
	const xml::Element* properties,
	AssetManager* asset_manager)
{
	ComponentType component;
	if( ComponentType::InitComponent(asset_manager, properties, &component) )
	{
		components_[entity] = component;
		return true;
	}
	return false;
}

template<class ComponentType>
inline bool ComponentMap<ComponentType>::DeleteComponent(
	Scene::EntityID entity)
{
	auto it = components_.find(entity);
	if( it != components_.end() )
	{
		components_.erase(it);
		return true;
	}
	return false;
}

template<class ComponentType>
inline std::map<Scene::EntityID, ComponentType>&
ComponentMap<ComponentType>::GetComponents()
{
	return components_;
}

template<class ComponentType>
inline ComponentType* ComponentMap<ComponentType>::GetComponent(
	Scene::EntityID entity)
{
	auto it = components_.find(entity);
	if( it != components_.end() )
		return &(it->second);

	return nullptr;
}

template<class ComponentType>
inline ComponentType* ComponentArray<ComponentType>::CreateComponent(
	Scene::EntityID entity)
{
	if( entity >= components_.size() )
		components_.resize(entity + 1);

	return &components_[entity];
}
template<class ComponentType>
inline bool ComponentArray<ComponentType>::CreateComponent(
	Scene::EntityID entity,
	const xml::Element* properties,
	AssetManager* asset_manager)
{
	ComponentType component;
	if( ComponentType::InitComponent(asset_manager, properties, &component) )
	{
		if( entity >= components_.size() )
			components_.resize(entity + 1);

		components_[entity] = component;
		return true;
	}
	return false;
}

template<class ComponentType>
inline bool ComponentArray<ComponentType>::DeleteComponent(
	Scene::EntityID entity)
{
	return true;
}

template<class ComponentType>
inline std::vector<ComponentType>&
ComponentArray<ComponentType>::GetComponents()
{
	return components_;
}

template<class ComponentType>
inline ComponentType* ComponentArray<ComponentType>::GetComponent(
	Scene::EntityID entity)
{
	assert(entity < components_.size());
	return &components_[entity];
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/