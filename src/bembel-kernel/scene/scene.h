/******************************************************************************/
/* ************************************************************************** */
/* *                                                                        * */
/* *    MIT License                                                         * */
/* *                                                                        * */
/* *   Copyright(c) 2018 Joachim Herber                                     * */
/* *                                                                        * */
/* *   Permission is hereby granted, free of charge, to any person          * */
/* *   obtaining copy of this software and associated documentation files   * */
/* *   (the "Software"), to deal in the Software without restriction,       * */
/* *   including without limitation the rights to use, copy, modify, merge, * */
/* *   publish, distribute, sublicense, and/or sell copies of the Software, * */
/* *   and to permit persons to whom the Software is furnished to do so,    * */
/* *   subject to the following conditions :                                * */
/* *                                                                        * */
/* *   The above copyright notice and this permission notice shall be       * */
/* *   included in all copies or substantial portions of the Software.      * */
/* *                                                                        * */
/* *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,      * */
/* *   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF   * */
/* *   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND                * */
/* *   NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS   * */
/* *   BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN   * */
/* *   ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN    * */
/* *   CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE     * */
/* *   SOFTWARE.                                                            * */
/* *                                                                        * */
/* ************************************************************************** */
/******************************************************************************/

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
namespace bembel {

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
	using EntityID = size_t;
	using ComponentTypeID = size_t;
	using ComponentMask = unsigned long long;

	enum
	{
		INVALID_ENTITY = ~EntityID(0)
	};

	Scene(AssetManager*);
	Scene(const Scene&) = delete;
	Scene& operator=(const Scene&) = delete;
	~Scene();

	template<class ComponentType>
	typename ComponentType::ContainerType* RequestComponentContainer();

	template<class ComponentType>
	void RegisterComponentType();

	EntityID CreateEntity();
	EntityID CreateEntity(const xml::Element*);
	bool DeleteEntity(EntityID);

	bool LoadScene(const std::string& file_name);

	template<class ComponentType>
	ComponentType* CreateComponent(EntityID id);

	template<class ComponentType>
	ComponentType* GetComponent(EntityID id);

	const std::vector<ComponentMask>& GetEntitys() const;

	template<typename AssetType>
	AssetHandle GetAssetHandle(const std::string& name);

	template<typename AssetType>
	AssetType* GetAsset(AssetHandle handle, bool return_dummy_if_handle_invalid = true);

	bool LoadAssets(const std::string& file);

private:
	void LoadAsset(const xml::Element*);

private:
	using ContainerPtr = std::unique_ptr<ComponentContainerBase>;

	std::vector<ComponentMask> entities_;
	std::stack<EntityID>       unused_entity_ids_;

	std::map<std::string, ComponentTypeID> component_type_map_;
	std::vector<ContainerPtr>              container_;

	AssetManager* asste_manager_;

	std::set<AssetHandle> assets_;
};

} //end of namespace bembel
/*============================================================================*/
/* INLINE IMPLEMENTATIONS													  */
/*============================================================================*/
namespace bembel {

template<class ComponentType>
inline typename ComponentType::ContainerType* Scene::RequestComponentContainer()
{
	auto it = component_type_map_.find(ComponentType::GetComponentTypeName());
	if( it != component_type_map_.end() )
	{
		return static_cast<ComponentType::ContainerType*>(
			container_[it->second].get());
	}

	auto container =
		std::make_unique<ComponentType::ContainerType>(container_.size());
	auto container_pointer = container.get();

	component_type_map_.emplace(
		ComponentType::GetComponentTypeName(), container_.size());
	container_.push_back(std::move(container));
	return container_pointer;
}

template<class ComponentType>
inline void Scene::RegisterComponentType()
{
	auto it = component_type_map_.find(ComponentType::GetComponentTypeName());
	if( it != component_type_map_.end() )
		return;

	auto container =
		std::make_unique<ComponentType::ContainerType>(container_.size());

	component_type_map_.emplace(
		ComponentType::GetComponentTypeName(), container_.size());
	container_.push_back(std::move(container));
}

template<class ComponentType>
ComponentType* Scene::CreateComponent(EntityID id)
{
	if( id >= entities_.size() )
		return nullptr;

	auto it = component_type_map_.find(ComponentType::GetComponentTypeName());
	if( it == component_type_map_.end() )
		return nullptr;

	auto container = static_cast<ComponentType::ContainerType*>(
		container_[it->second].get());

	entities_[id] |= container->GetComponentMask();
	return container->CreateComponent(id);
}

template<class ComponentType>
ComponentType* Scene::GetComponent(EntityID id)
{
	if( id >= entities_.size() )
		return nullptr; // invalided entity id

	auto it = component_type_map_.find(ComponentType::GetComponentTypeName());
	if( it == component_type_map_.end() )
		return nullptr; // component type does not exist

	auto container = static_cast<ComponentType::ContainerType*>(
		container_[it->second].get());

	if( (entities_[id] & container->GetComponentMask()) == 0 )
		return nullptr; // entity doesn't have a component of the requested type

	return container->GetComponent(id);
}

template<typename AssetType>
inline AssetHandle Scene::GetAssetHandle(const std::string & name)
{
	return asste_manager_->GetAssetHandle<AssetType>(name);
}

template<typename AssetType>
inline AssetType* Scene::GetAsset(AssetHandle handle, bool returnDummyIfHandleInvalid)
{
	return asste_manager_->GetAsset<AssetType>(handle, returnDummyIfHandleInvalid);
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
