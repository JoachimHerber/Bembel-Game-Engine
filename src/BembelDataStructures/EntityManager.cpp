/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "EntityManager.h"
#include "ComponentContainer.hpp"
#include "BembelBase\Logging\Logger.h"

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel {

EntityManager::EntityManager()
{}

EntityManager::~EntityManager()
{}

EntityManager::EntityID EntityManager::CreateEntity()
{
	for (EntityID entity = 0; entity< _entities.size(); ++entity)
	{
		if (_entities[entity] == 0)
			return entity;
	}
	_entities.push_back(0);
	return _entities.size() - 1;
}

EntityManager::EntityID EntityManager::CreateEntity(
	const xml::Element* properties)
{
	EntityID entity = CreateEntity();
	for (const xml::Element* component : xml::IterateChildElements(properties))
	{
		auto it = _componentTypeMap.find(component->Value());
		if(it == _componentTypeMap.end())
			continue; // unknow component type

		if (_container[it->second]->CreateComponent(entity, component))
		{
			_entities[entity] |= _container[it->second]->GetComponentMask();
		}
	}
	return entity;
}

bool EntityManager::LoadEntities(const std::string& fileName)
{
	xml::Document doc;
	if (doc.LoadFile(fileName.c_str()) != tinyxml2::XML_SUCCESS)
	{
		BEMBEL_LOG_ERROR()
			<< "Failed to lode file '" << fileName << "'\n"
			<< doc.ErrorName() << std::endl;
		return false;
	}

	const xml::Element* root = doc.FirstChildElement("Entities");
	if (!root)
		return false;

	for (auto entity : xml::IterateChildElements(root, "Entity"))
	{
		CreateEntity(entity);
	}
}

const std::vector<EntityManager::ComponentMask>& EntityManager::GetEntitys() const
{
	return _entities;
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
