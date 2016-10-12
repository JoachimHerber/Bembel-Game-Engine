/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "Scene.h"
#include "ComponentContainer.hpp"
#include "../Assets/AssetManager.h"
#include "BembelBase/Logging/Logger.h"

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel {

Scene::Scene() : 
	_assteManager(std::make_shared<AssetManager>())
{}
Scene::Scene(std::shared_ptr<AssetManager> assetMgr)
	: _assteManager(assetMgr)
{}

Scene::~Scene()
{}

Scene::EntityID Scene::CreateEntity()
{
	for (EntityID entity = 0; entity< _entities.size(); ++entity)
	{
		if (_entities[entity] == 0)
			return entity;
	}
	_entities.push_back(0);
	return _entities.size() - 1;
}

Scene::EntityID Scene::CreateEntity(
	const xml::Element* properties)
{
	EntityID entity = CreateEntity();
	for (const xml::Element* component : xml::IterateChildElements(properties))
	{
		auto it = _componentTypeMap.find(component->Value());
		if(it == _componentTypeMap.end())
			continue; // unknow component type

		if (_container[it->second]->CreateComponent(entity, component, _assteManager.get()))
		{
			_entities[entity] |= _container[it->second]->GetComponentMask();
		}
	}
	return entity;
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

std::shared_ptr<AssetManager> Scene::GetAssetManager()
{
	return _assteManager;
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
