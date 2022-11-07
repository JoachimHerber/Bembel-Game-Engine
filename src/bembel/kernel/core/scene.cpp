module;
#include "bembel/pch.h"
module bembel.kernel.core;

import bembel.base;

namespace bembel::kernel {
using namespace bembel::base;

Scene::~Scene() {
    for(auto asset_handle : m_assets) { m_asste_mgr.releaseAsset(asset_handle); }
}

EntityID Scene::createEntity() {
    if(m_unused_entity_ids.empty()) {
        m_entities.push_back(0);
        return EntityID(m_entities.size() - 1);
    } else {
        EntityID id = m_unused_entity_ids.top();
        m_unused_entity_ids.pop();
        return id;
    }
}

EntityID Scene::createEntity(xml::Element const* properties) {
    EntityID entity_id = createEntity();
    for(xml::Element const* component : xml::IterateChildElements(properties)) {
        auto it = m_component_type_map.find(component->Value());
        if(it == m_component_type_map.end()) continue; // unknown component type

        if(m_container[it->second]->createComponent(entity_id, component, m_asste_mgr)) {
            m_entities[u64(entity_id)] |= m_container[it->second]->getComponentMask();
        }
    }
    return entity_id;
}

bool Scene::deleteEntity(EntityID entity_id) {
    if(u64(entity_id) >= m_entities.size()) return false; // invalid entity id

    // delete all components of of the entity
    for(auto& container : m_container) {
        if((m_entities[u64(entity_id)] & container->getComponentMask()) != 0)
            container->deleteComponent(entity_id);
    }

    m_entities[u64(entity_id)] = 0;
    m_unused_entity_ids.push(entity_id);
    return true;
}

bool Scene::loadScene(std::filesystem::path file) {
    std::string const file_path = file.string(); // file.c_str() returns a wchar*
    xml::Document     doc;
    if(doc.LoadFile(file_path.c_str()) != tinyxml2::XML_SUCCESS) {
        log().error("Failed to lode file '{}'\n{}", file_path, doc.ErrorName());
        return false;
    }

    xml::Element const* root = doc.FirstChildElement("Scene");
    if(!root) return false;

    xml::Element const* assets = root->FirstChildElement("Assets");
    for(auto asset : xml::IterateChildElements(assets)) { loadAsset(asset); }

    xml::Element const* entities = root->FirstChildElement("Entities");
    for(auto entity : xml::IterateChildElements(entities, "Entity")) { createEntity(entity); }
    return true;
}

std::vector<ComponentMask> const& Scene::getEntitys() const {
    return m_entities;
}

bool Scene::loadAssets(std::filesystem::path file) {
    std::string const file_path_str = file.string(); // file.c_str() returns a wchar*
    xml::Document     doc;
    if(doc.LoadFile(file_path_str.c_str()) != tinyxml2::XML_SUCCESS) {
        log().error("Failed to lode file '{}'\n{}", file_path_str, doc.ErrorName());
        return false;
    }

    xml::Element const* root = doc.FirstChildElement("Assets");
    if(!root) {
        log().error("File '{}' has no root element 'Assets'", file_path_str);
        return false;
    }
    for(auto it : xml::IterateChildElements(root)) { loadAsset(it); }
    return true;
}

void Scene::loadAsset(xml::Element const* properties) {
    AssetHandle hndl = m_asste_mgr.requestAsset(properties->Value(), properties);

    if(m_asste_mgr.isHandelValid(hndl)) { m_assets.emplace(hndl); }
}

} // namespace bembel::kernel
