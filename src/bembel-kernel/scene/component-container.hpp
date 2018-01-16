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

#ifndef BEMBEL_KERNEL_SCENE_COMPONENCONTAINER_H_
#define BEMBEL_KERNEL_SCENE_COMPONENCONTAINER_H_
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "bembel-config.h"
#include "scene.h"

#include <vector>
#include <map>

#include <bembel-base/xml.h>

/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel {

class BEMBEL_API ComponentContainerBase
{
public:
	ComponentContainerBase(
		Scene::ComponentTypeID
	);
	virtual ~ComponentContainerBase()
	{}

	virtual bool CreateComponent(
		Scene::EntityID, const xml::Element*, AssetManager*) = 0;
	virtual bool DeleteComponent(Scene::EntityID) = 0;

	Scene::ComponentTypeID GetComponentTypeID();
	Scene::ComponentMask   GetComponentMask();

private:
	Scene::ComponentTypeID type_id_;
	Scene::ComponentMask   mask_;
};

template<class ComponentType>
class ComponentMap : public ComponentContainerBase
{
public:
	ComponentMap(
		Scene::ComponentTypeID id)
		: ComponentContainerBase(id)
	{}
	virtual ~ComponentMap()
	{}

	ComponentType* CreateComponent(Scene::EntityID);
	bool CreateComponent(Scene::EntityID, const xml::Element*, AssetManager*) override;
	bool DeleteComponent(Scene::EntityID) override;

	std::map<Scene::EntityID, ComponentType>& GetComponents();
	ComponentType* GetComponent(Scene::EntityID);

private:
	std::map<Scene::EntityID, ComponentType> components_;
};

template<class ComponentType>
class ComponentArray : public ComponentContainerBase
{
public:
	ComponentArray(
		Scene::ComponentTypeID id)
		: ComponentContainerBase(id)
	{}
	virtual ~ComponentArray()
	{}

	ComponentType* CreateComponent(Scene::EntityID);
	bool CreateComponent(Scene::EntityID, const xml::Element*, AssetManager*) override;
	bool DeleteComponent(Scene::EntityID) override;

	std::vector<ComponentType>& GetComponents();
	ComponentType* GetComponent(Scene::EntityID);

private:
	std::vector<ComponentType> components_;
};
} //end of namespace bembel
/*============================================================================*/
/* INLINE IMPLEMENTATIONS													  */
/*============================================================================*/

#include "component-container.inl"

/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
