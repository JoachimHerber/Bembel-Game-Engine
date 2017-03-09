#ifndef BEMBEL_KERNEL_SCENE_SCENE_H_
#define BEMBEL_KERNEL_SCENE_SCENE_H_
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "bembel-config.h"

#include <memory>
#include <vector>
#include <stack>
#include <map>
#include <set>

#include <bembel-base/xml.h>
#include <bembel-kernel/assets/asset-handle.h>

/*============================================================================*/
/* FORWARD DECLARATIONS                                                       */
/*============================================================================*/
namespace bembel{

class ComponentContainerBase;
class AssetManager;

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

	enum{ INVALID_ENTITY = ~EntityID(0) };

	Scene(AssetManager*);
	~Scene();

	template<class ComponentType>
	std::shared_ptr<typename ComponentType::ContainerType>
		RequestComponentContainer();

	template<class ComponentType>
	void RegisterComponentType();

	EntityID CreateEntity();
	EntityID CreateEntity(const xml::Element*);
	bool DeleteEntity(EntityID);

	bool LoadScene(const std::string& fileName);

	template<class ComponentType>
	ComponentType* CreateComponent(EntityID id);
	
	template<class ComponentType>
	ComponentType* GetComponent(EntityID id);

	const std::vector<ComponentMask>& GetEntitys() const;

	template<typename AssetType>
	AssetHandle GetAssetHandle( const std::string& name );

	template<typename AssetType>
	AssetType* GetAsset( AssetHandle handle, bool returnDummyIfHandleInvalid = true );

	bool LoadAssets(const std::string& file);

private:
	void LoadAsset( const xml::Element* );

private:
	using ContainerPtr = std::shared_ptr<ComponentContainerBase>;

	std::vector<ComponentMask> _entities;
	std::stack<EntityID>       _unusedEntityIds;

	std::map<std::string, ComponentTypeID> _componentTypeMap;
	std::vector<ContainerPtr>              _container;

	AssetManager* _assteManager;

	std::set<AssetHandle> _assets;
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
inline void Scene::RegisterComponentType()
{
	auto it = _componentTypeMap.find(ComponentType::GetComponentTypeName());
	if (it != _componentTypeMap.end())
		return;

	auto container =
		std::make_shared<ComponentType::ContainerType>(_container.size());

	_componentTypeMap.emplace(
		ComponentType::GetComponentTypeName(), _container.size());
	_container.push_back(container);
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

template<class ComponentType>
ComponentType* Scene::GetComponent(EntityID id)
{
	if (id >= _entities.size())
		return nullptr; // invalided entity id

	auto it = _componentTypeMap.find(ComponentType::GetComponentTypeName());
	if (it == _componentTypeMap.end())
		return nullptr; // component type does not exist

	auto container = std::static_pointer_cast<ComponentType::ContainerType>(
		_container[it->second]);

	if ((_entities[id] & container->GetComponentMask()) == 0)
		return nullptr; // entity doesn't have a component of the requested type

	return container->GetComponent(id);
}

template<typename AssetType>
inline AssetHandle Scene::GetAssetHandle( const std::string & name )
{
	return _assteManager->GetAssetHandle<AssetType>( name );
}

template<typename AssetType>
inline AssetType* Scene::GetAsset( AssetHandle handle, bool returnDummyIfHandleInvalid )
{
	return _assteManager->GetAsset<AssetType>( handle, returnDummyIfHandleInvalid );
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
