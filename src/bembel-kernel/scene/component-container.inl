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