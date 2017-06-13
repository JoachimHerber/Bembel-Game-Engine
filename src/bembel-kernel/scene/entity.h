/******************************************************************************/
/* ************************************************************************** */
/* *                                                                        * */
/* *    MIT License                                                         * */
/* *                                                                        * */
/* *   Copyright(c) 2017 Joachim Herber                                     * */
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

#ifndef BEMBEL_KERNEL_SCENE_ENTITY_H_
#define BEMBEL_KERNEL_SCENE_ENTITY_H_
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "bembel-config.h"
#include "scene.h"

/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel {

class BEMBEL_API Entity
{
public:
	Entity(Scene*, Scene::EntityID);
	Entity(const Entity&);
	Entity(Entity&&);

	template<class ComponentType>
	bool HasComponent();

	template<class ComponentType>
	ComponentType* CreateComponent();

	template<class ComponentType>
	ComponentType* GetComponent();

	static Entity CreateEntity(Scene*);

private:
	Scene*          _world;
	Scene::EntityID _id;
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
