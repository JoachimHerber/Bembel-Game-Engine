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

Scene::Scene(AssetManager* asset_manager)
	: asste_manager_(asset_manager)
{}

Scene::~Scene()
{
	for( auto asset_handle : assets_ )
	{
		asste_manager_->ReleaseAsset(asset_handle);
	}
}

Scene::EntityID Scene::CreateEntity()
{
	if( unused_entity_ids_.empty() )
	{
		entities_.push_back(0);
		return entities_.size() - 1;
	}
	else
	{
		EntityID id = unused_entity_ids_.top();
		unused_entity_ids_.pop();
		return id;
	}
}

Scene::EntityID Scene::CreateEntity(
	const xml::Element* properties)
{
	EntityID entity = CreateEntity();
	for( const xml::Element* component : xml::IterateChildElements(properties) )
	{
		auto it = component_type_map_.find(component->Value());
		if( it == component_type_map_.end() )
			continue; // unknown component type

		if( container_[it->second]->CreateComponent(entity, component, asste_manager_) )
		{
			entities_[entity] |= container_[it->second]->GetComponentMask();
		}
	}
	return entity;
}

bool Scene::DeleteEntity(EntityID id)
{
	if( id >= entities_.size() )
		return false; // invalid entity id

	// delete all components of of the entity
	for( auto& container : container_ )
	{
		if( entities_[id] & container->GetComponentMask() != 0 )
			container->DeleteComponent(id);
	}

	entities_[id] = 0;
	unused_entity_ids_.push(id);
}

bool Scene::LoadScene(const std::string& file_name)
{
	xml::Document doc;
	if( doc.LoadFile(file_name.c_str()) != tinyxml2::XML_SUCCESS )
	{
		BEMBEL_LOG_ERROR()
			<< "Failed to lode file '" << file_name << "'\n"
			<< doc.ErrorName() << std::endl;
		return false;
	}

	const xml::Element* root = doc.FirstChildElement("Scene");
	if( !root )
		return false;

	const xml::Element* assets = root->FirstChildElement("Assets");
	for( auto asset : xml::IterateChildElements(assets) )
	{
		LoadAsset(asset);
	}

	const xml::Element* entities = root->FirstChildElement("Entities");
	for( auto entity : xml::IterateChildElements(entities, "Entity") )
	{
		CreateEntity(entity);
	}
	return true;
}

const std::vector<Scene::ComponentMask>& Scene::GetEntitys() const
{
	return entities_;
}

bool Scene::LoadAssets(const std::string& file_name)
{
	xml::Document doc;
	if( doc.LoadFile(file_name.c_str()) != tinyxml2::XML_SUCCESS )
	{
		BEMBEL_LOG_ERROR()
			<< "Failed to load file '" << file_name << "'\n"
			<< doc.ErrorName() << std::endl;
		return false;
	}

	const xml::Element* root = doc.FirstChildElement("Assets");
	if( !root )
	{
		BEMBEL_LOG_ERROR()
			<< "File '" << file_name << "' has no root element 'GeometryMesh'"
			<< std::endl;
		return false;
	}
	for( auto it : xml::IterateChildElements(root) )
	{
		LoadAsset(it);
	}
	return true;
}

void Scene::LoadAsset(const xml::Element* properties)
{
	AssetHandle hndl = asste_manager_->RequestAsset(
		properties->Value(), properties);

	if( asste_manager_->IsHandelValid(hndl) )
	{
		assets_.emplace(hndl);
	}
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
