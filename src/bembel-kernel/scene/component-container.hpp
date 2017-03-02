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
	virtual ~ComponentContainerBase(){}

	virtual bool CreateComponent(
		Scene::EntityID, const xml::Element*, AssetManager*) = 0;
	virtual bool DeleteComponent(Scene::EntityID) = 0;

	Scene::ComponentTypeID GetComponentTypeID();
	Scene::ComponentMask   GetComponentMask();

private:
	Scene::ComponentTypeID _typeID;
	Scene::ComponentMask   _mask;
};

template<class ComponentType>
class SparseComponentContainer : public ComponentContainerBase
{
public:
	SparseComponentContainer(
		Scene::ComponentTypeID id)
		: ComponentContainerBase(id)
	{}
	virtual ~SparseComponentContainer() {}

	ComponentType* CreateComponent(Scene::EntityID);
	bool CreateComponent(Scene::EntityID, const xml::Element*, AssetManager*) override;
	bool DeleteComponent(Scene::EntityID) override;

	std::map<Scene::EntityID, ComponentType>& GetComponents();
	ComponentType* GetComponent(Scene::EntityID);

private:
	std::map<Scene::EntityID, ComponentType> _components;
};

template<class ComponentType>
class DenseComponentContainer : public ComponentContainerBase
{
public:
	DenseComponentContainer(
		Scene::ComponentTypeID id)
		: ComponentContainerBase(id)
	{}
	virtual ~DenseComponentContainer() {}

	ComponentType* CreateComponent(Scene::EntityID);
	bool CreateComponent(Scene::EntityID, const xml::Element*, AssetManager*) override;
	bool DeleteComponent(Scene::EntityID) override;

	std::vector<ComponentType>& GetComponents();
	ComponentType* GetComponent(Scene::EntityID);

private:
	std::vector<ComponentType> _components;
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
