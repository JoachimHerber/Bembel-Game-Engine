#include "./scene.hpp"

#include <bembel/base/logging/logger.hpp>

#include "./component-container.hpp"

namespace bembel::kernel {

Scene::Scene(AssetManager& asste_mgr)
: asste_mgr(asste_mgr) {
}

Scene::~Scene() {
    for(auto asset_handle : this->assets) { this->asste_mgr.releaseAsset(asset_handle); }
}

Scene::EntityID Scene::createEntity() {
    if(this->unused_entity_ids.empty()) {
        this->entities.push_back(0);
        return this->entities.size() - 1;
    } else {
        EntityID id = this->unused_entity_ids.top();
        this->unused_entity_ids.pop();
        return id;
    }
}

Scene::EntityID Scene::createEntity(const xml::Element* properties) {
    EntityID entity = createEntity();
    for(const xml::Element* component : xml::IterateChildElements(properties)) {
        auto it = this->component_type_map.find(component->Value());
        if(it == this->component_type_map.end()) continue; // unknown component type

        if(this->container[it->second]->createComponent(entity, component, this->asste_mgr)) {
            this->entities[entity] |= this->container[it->second]->getComponentMask();
        }
    }
    return entity;
}

bool Scene::deleteEntity(EntityID id) {
    if(id >= this->entities.size()) return false; // invalid entity id

    // delete all components of of the entity
    for(auto& container : this->container) {
        if((this->entities[id] & container->getComponentMask()) != 0)
            container->deleteComponent(id);
    }

    this->entities[id] = 0;
    this->unused_entity_ids.push(id);
    return true;
}

bool Scene::loadScene(const std::string& file_name) {
    xml::Document doc;
    if(doc.LoadFile(file_name.c_str()) != tinyxml2::XML_SUCCESS) {
        BEMBEL_LOG_ERROR() << "Failed to lode file '" << file_name << "'\n"
                           << doc.ErrorName() << std::endl;
        return false;
    }

    const xml::Element* root = doc.FirstChildElement("Scene");
    if(!root) return false;

    const xml::Element* assets = root->FirstChildElement("Assets");
    for(auto asset : xml::IterateChildElements(assets)) { loadAsset(asset); }

    const xml::Element* entities = root->FirstChildElement("Entities");
    for(auto entity : xml::IterateChildElements(entities, "Entity")) { createEntity(entity); }
    return true;
}

const std::vector<Scene::ComponentMask>& Scene::getEntitys() const {
    return this->entities;
}

bool Scene::loadAssets(const std::string& file_name) {
    xml::Document doc;
    if(doc.LoadFile(file_name.c_str()) != tinyxml2::XML_SUCCESS) {
        BEMBEL_LOG_ERROR() << "Failed to load file '" << file_name << "'\n"
                           << doc.ErrorName() << std::endl;
        return false;
    }

    const xml::Element* root = doc.FirstChildElement("Assets");
    if(!root) {
        BEMBEL_LOG_ERROR() << "File '" << file_name << "' has no root element 'GeometryMesh'"
                           << std::endl;
        return false;
    }
    for(auto it : xml::IterateChildElements(root)) { loadAsset(it); }
    return true;
}

void Scene::loadAsset(const xml::Element* properties) {
    AssetHandle hndl = this->asste_mgr.requestAsset(properties->Value(), properties);

    if(this->asste_mgr.isHandelValid(hndl)) { this->assets.emplace(hndl); }
}

} // namespace bembel::kernel
