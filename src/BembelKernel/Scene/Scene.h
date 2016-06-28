#ifndef BEMBEL_SCENE_H
#define BEMBEL_SCENE_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include <BembelConfig.h>
#include <BembelBase/XML.h>

#include <memory>
#include <vector>
#include <map>

/*============================================================================*/
/* FORWARD DECLARATIONS                                                       */
/*============================================================================*/
namespace bembel{

class ComponentContainerBase;

}//end of namespace bembel
/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel {

class BEMBEL_API Scene
{
public:
	using EntityID        = size_t;
	using ComponentTypeID = size_t;
	using ComponentMask   = unsigned long long;

	Scene();
	~Scene();

	template<class ComponentType>
	std::shared_ptr<typename ComponentType::ContainerType>
		RequestComponentContainer();

	EntityID CreateEntity();
	EntityID CreateEntity(const xml::Element*);

	bool LoadEntities(const std::string& fileName);

	template<class ComponentType>
	ComponentType* CreateComponent(EntityID id);

	const std::vector<ComponentMask>& GetEntitys() const;

private:
	using ContainerPtr = std::shared_ptr<ComponentContainerBase>;

	std::vector<ComponentMask> _entities;

	std::map<std::string, ComponentTypeID> _componentTypeMap;
	std::vector<ContainerPtr>              _container;
};

} //end of namespace bembel
/*============================================================================*/
/* INLINE IMPLEMENTATIONS													  */
/*============================================================================*/
namespace bembel{

template<class ComponentType>
inline std::shared_ptr<typename ComponentType::ContainerType>
	Scene::RequestComponentContainer()
{
	auto it = _componentTypeMap.find(ComponentType::GetComponentTypeName());
	if (it != _componentTypeMap.end())
	{
		return std::static_pointer_cast<ComponentType::ContainerType>(
			_container[it->second]);
	}
		
	auto container = 
		std::make_shared<ComponentType::ContainerType>(_container.size());

	_componentTypeMap.emplace(
		ComponentType::GetComponentTypeName(), _container.size());
	_container.push_back(container);
	return container;
}

template<class ComponentType>
ComponentType* Scene::CreateComponent(EntityID id)
{
	if (id >= _entities.size())
		return nullptr;

	auto it = _componentTypeMap.find(ComponentType::GetComponentTypeName());
	if (it == _componentTypeMap.end())
		return nullptr;

	auto container = std::static_pointer_cast<ComponentType::ContainerType>(
		_container[it->second]);

	_entities[id] |= container->GetComponentMask();
	return container->CreateComponent(id);
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
