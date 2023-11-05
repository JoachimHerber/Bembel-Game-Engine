module;
#include <btBulletDynamicsCommon.h>

#include <glm/glm.hpp>
#include <memory>

module bembel.physics;

import bembel.base;
import bembel.kernel;

namespace bembel::physics {
using namespace bembel::base;
using namespace bembel::kernel;

std::unique_ptr<CollisionShape> CollisionShape::loadAsset(std::filesystem::path file) {
    std::string const file_path = file.string(); // file.c_str() returns a wchar*
    xml::Document     doc;
    if(doc.LoadFile(file_path.c_str()) != tinyxml2::XML_SUCCESS) {
        logError("Failed to load file '{}' \n {}", file_path, doc.ErrorName());
        return nullptr;
    }

    xml::Element const* root = doc.FirstChildElement("CollisionShape");
    if(!root) {
        logError("File '{}' has no root element 'CollisionShape'", file_path);
        return nullptr;
    }
    return CollisionShape::createCollisionShape(root);
}

std::unique_ptr<CollisionShape> CollisionShape::createAsset(xml::Element const* properties) {
    return CollisionShape::createCollisionShape(properties);
}

void CollisionShape::deleteAsset(std::unique_ptr<CollisionShape>) {}

void CollisionShape::initFactory() {
    auto& factory = CollisionShape::getFactory();

    factory.registerCustomObjectGenerator("box", [](xml::Element const* params) {
        vec3 size = {1, 1, 1};
        xml::getAttribute(params, "size", size);
        return std::make_unique<CollisionBox>(btVector3{size.x / 2, size.y / 2, size.z / 2});
    });

    factory.registerCustomObjectGenerator("sphere", [](xml::Element const* params) {
        float radius = 0.5f;
        xml::getAttribute(params, "radius", radius);
        return std::make_unique<CollisionSphere>(radius);
    });
    factory.registerCustomObjectGenerator("cylinder", [](xml::Element const* params) {
        float radius = 0.5f;
        float height = 1.0f;
        xml::getAttribute(params, "radius", radius);
        xml::getAttribute(params, "height", height);
        return std::make_unique<CollisionCylinder>(btVector3{radius, height / 2, radius});
    });
    factory.registerCustomObjectGenerator("cone", [](xml::Element const* params) {
        float radius = 0.5f;
        float height = 1.0f;
        xml::getAttribute(params, "radius", radius);
        xml::getAttribute(params, "height", height);
        return std::make_unique<CollisionCone>(radius, height);
    });

    factory.registerCustomObjectGenerator("compound", [](xml::Element const* params) {
        auto shape = std::make_unique<CollisionCompoundShape>();
        for(auto* child : xml::IterateChildElements(params, "ChildShape")) {
            vec3        pos{0, 0, 0};
            quat        rot{1, 0, 0, 0};
            std::string type;
            xml::getAttribute(child, "position", pos);
            if(!xml::getAttribute(child, "rotation", rot)) {
                float roll  = 0;
                float pitch = 0;
                float yaw   = 0;
                if(xml::getAttribute(child, "roll", roll)
                   || xml::getAttribute(child, "pitch", pitch)
                   || xml::getAttribute(child, "yaw", yaw)) {
                    rot = quat(vec3(glm::radians(roll), glm::radians(yaw), glm::radians(pitch)));
                }
            }
            xml::getAttribute(child, "type", type);
            shape->addChildShape(getFactory().createObject(type, child), pos, rot);
        }
        return std::move(shape);
    });
}

std::unique_ptr<CollisionShape> CollisionShape::createCollisionShape(xml::Element const* properties
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
