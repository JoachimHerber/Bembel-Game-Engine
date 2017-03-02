/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "scene.h"
#include "component-container.hpp"
#include "../assets/asset-manager.h"

#include <bembel-base/logging/logger.h>

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel {

Scene::Scene(AssetManager* assetMgr)
	: _assteManager(assetMgr)
{}

Scene::~Scene()
{}

Scene::EntityID Scene::CreateEntity()
{
	if (_unusedEntityIds.empty())
	{
		_entities.push_back(0);
		return _entities.size() - 1;
	}
	else
	{
		EntityID id = _unusedEntityIds.top();
		_unusedEntityIds.pop();
		return id;
	}
}

Scene::EntityID Scene::CreateEntity(
	const xml::Element* properties)
{
	EntityID entity = CreateEntity();
	for (const xml::Element* component : xml::IterateChildElements(properties))
	{
		auto it = _componentTypeMap.find(component->Value());
		if(it == _componentTypeMap.end())
			continue; // unknown component type

		if (_container[it->second]->CreateComponent(entity, component, _assteManager))
		{
			_entities[entity] |= _container[it->second]->GetComponentMask();
		}
	}
	return entity;
}

bool Scene::DeleteEntity(EntityID id)
{
	if (id >= _entities.size())
		return false; // invalid entity id

	// delete all components of of the entity
	for (auto& container : _container)
	{
		if (_entities[id] & container->GetComponentMask() != 0)
			container->DeleteComponent(id);
	}

	_entities[id] = 0;
	_unusedEntityIds.push(id);
}

bool Scene::LoadScene(const std::string& fileName)
{
	xml::Document doc;
	if (doc.LoadFile(fileName.c_str()) != tinyxml2::XML_SUCCESS)
	{
		BEMBEL_LOG_ERROR()
			<< "Failed to lode file '" << fileName << "'\n"
			<< doc.ErrorName() << std::endl;
		return false;
	}

	const xml::Element* root = doc.FirstChildElement("Scene");
	if (!root)
		return false;

	const xml::Element* entities = root->FirstChildElement("Entities");
	for (auto entity : xml::IterateChildElements(entities, "Entity"))
	{
		CreateEntity(entity);
	}
	return true;
}

const std::vector<Scene::ComponentMask>& Scene::GetEntitys() const
{
	return _entities;
}

AssetManager* Scene::GetAssetManager()
{
	return _assteManager;
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
