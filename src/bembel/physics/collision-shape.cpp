module;
#include <bullet/btBulletDynamicsCommon.h>

#include <memory>
module bembel.physics;

import bembel.base;
import bembel.kernel;

namespace bembel::physics {
using namespace bembel::base;
using namespace bembel::kernel;

std::unique_ptr<CollisionShape> CollisionShape::createBox(vec3 size) {
    return std::make_unique<TCollisionShape<btBoxShape>>(btVector3(size.x, size.y, size.z));
}

std::unique_ptr<CollisionShape> CollisionShape::createSphere(float radius) {
    return std::make_unique<TCollisionShape<btSphereShape>>(radius);
}

std::unique_ptr<CollisionShape> CollisionShape::loadAsset(
    AssetManager& asset_mgr, std::filesystem::path file
) {
    std::string const file_path = file.string(); // file.c_str() returns a wchar*
    xml::Document     doc;
    if(doc.LoadFile(file_path.c_str()) != tinyxml2::XML_SUCCESS) {
        log().error("Failed to load file '{}' \n {}", file_path, doc.ErrorName());
        return nullptr;
    }

    xml::Element const* root = doc.FirstChildElement("CollisionShape");
    if(!root) {
        log().error("File '{}' has no root element 'CollisionShape'", file_path);
        return nullptr;
    }
    return CollisionShape::createCollisionShape(asset_mgr, root);
}

std::unique_ptr<CollisionShape> CollisionShape::createAsset(
    AssetManager& asset_mgr, xml::Element const* properties
) {
    return CollisionShape::createCollisionShape(asset_mgr, properties);
}

void CollisionShape::deleteAsset(AssetManager& asset_mgr, std::unique_ptr<CollisionShape> model) {}

void CollisionShape::initFactory() {
    auto& factory = CollisionShape::getFactory();

    factory.registerCustomObjectGenerator("box", [](xml::Element const* params) {
        vec3 size = {1, 1, 1};
        xml::getAttribute(params, "size", size);
        return createBox(size);
    });

    factory.registerCustomObjectGenerator("sphere", [](xml::Element const* params) {
        float radius = 0.5f;
        xml::getAttribute(params, "radius", radius);
        return createSphere(radius);
    });
}

std::unique_ptr<CollisionShape> CollisionShape::createCollisionShape(
    AssetManager& asset_mgr, xml::Element const* properties
) {
    std::string type;
    xml::getAttribute(properties, "type", type);
    return getFactory().createObject(type, properties);
}

Factory<CollisionShape, xml::Element const*>& CollisionShape::getFactory() {
    static Factory<CollisionShape, xml::Element const*> factory;
    return factory;
}

} // namespace bembel::physics
