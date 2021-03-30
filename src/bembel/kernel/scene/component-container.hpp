#ifndef BEMBEL_KERNEL_SCENE_COMPONENCONTAINER_HPP
#define BEMBEL_KERNEL_SCENE_COMPONENCONTAINER_HPP

#include <bembel/base/io/xml.hpp>

#include "./scene.hpp"

namespace bembel::kernel {

class BEMBEL_API ComponentContainerBase {
  public:
    ComponentContainerBase(Scene::ComponentTypeID);
    virtual ~ComponentContainerBase() {}

    virtual bool createComponent(Scene::EntityID, const base::xml::Element*, AssetManager&) = 0;
    virtual bool deleteComponent(Scene::EntityID)                                           = 0;

    Scene::ComponentTypeID getComponentTypeID();
    Scene::ComponentMask   getComponentMask();

  private:
    Scene::ComponentTypeID type_id;
    Scene::ComponentMask   mask;
};

template <class ComponentType>
class ComponentMap : public ComponentContainerBase {
  public:
    ComponentMap(Scene::ComponentTypeID id)
    : ComponentContainerBase(id) {}
    virtual ~ComponentMap() {}

    ComponentType* createComponent(Scene::EntityID);
    bool createComponent(Scene::EntityID, const base::xml::Element*, AssetManager&) override;
    bool deleteComponent(Scene::EntityID) override;

    std::map<Scene::EntityID, ComponentType>& getComponents();
    ComponentType*                            getComponent(Scene::EntityID);

  private:
    std::map<Scene::EntityID, ComponentType> components;
};

template <class ComponentType>
class ComponentArray : public ComponentContainerBase {
  public:
    ComponentArray(Scene::ComponentTypeID id)
    : ComponentContainerBase(id) {}
    virtual ~ComponentArray() {}

    ComponentType* createComponent(Scene::EntityID);
    bool           createComponent(Scene::EntityID, const xml::Element*, AssetManager&) override;
    bool           deleteComponent(Scene::EntityID) override;

    std::vector<ComponentType>& getComponents();
    ComponentType*              getComponent(Scene::EntityID);

  private:
    std::vector<ComponentType> components;
};

} // namespace bembel::kernel
#include "component-container.inl"
#endif // include guards
