export module bembel.examples.gm_helper:Components;

import std;
import bembel;

namespace bembel::examples::gm_helper {
using namespace base;
using namespace kernel;
using namespace graphics;

export struct Selectable {
    float dist;
    bool  selected = false;

    using Container = ComponentVector<Selectable>;

    static bool serialize(Selectable const& data, xml::Element* entity) {
        auto* properties = entity->GetDocument()->NewElement("Selectable");
        entity->InsertEndChild(properties);

        xml::setAttribute(properties, "dist", data.dist);
        return true;
    }

    static bool deserialize(Container* container, EntityID entity_id, xml::Element const* entity) {
        auto* properties = entity->FirstChildElement("Selectable");
        if(!properties) return false;

        float dist = 0.f;
        if(!xml::getAttribute(properties, "dist", dist)) return false;

        container->assignComponent(entity_id, dist);
        return true;
    }
};

export struct Token {
    enum Size : uint { Tiny, Small, Medium, Large, Huge, Gargantuan };

    Size           size;
    vec4           color;
    std::string    name;
    Asset<Texture> texture;
    bool           hidden    = false;
    bool           is_player = false;

    using Container = ComponentVector<Token>;

    static bool serialize(Token const& data, xml::Element* entity) {
        auto* properties = entity->GetDocument()->NewElement("Token");
        entity->InsertEndChild(properties);

        std::string texture{data.texture.getAlias()};
        if(texture.empty()) return false;

        xml::setAttribute(properties, "size", uint(data.size));
        xml::setAttribute(properties, "name", data.name);
        xml::setAttribute(properties, "texture", texture);
        xml::setAttribute(properties, "hidden", data.hidden);
        xml::setAttribute(properties, "color", data.color);
        xml::setAttribute(properties, "is_player", data.is_player);
        return true;
    }

    static bool deserialize(Container* container, EntityID entity_id, xml::Element const* entity) {
        auto* properties = entity->FirstChildElement("Token");
        if(!properties) return false;

        uint        size;
        vec4        color;
        std::string name;
        std::string texture;
        bool        hidden    = false;
        bool        is_player = false;
        xml::getAttribute(properties, "size", size);
        xml::getAttribute(properties, "name", name);
        xml::getAttribute(properties, "texture", texture);
        xml::getAttribute(properties, "hidden", hidden);
        xml::getAttribute(properties, "color", color);
        xml::getAttribute(properties, "is_player", is_player);

        Asset<Texture> tx{texture};
        if(!tx) return false;

        container->assignComponent(entity_id, Size(size), color, name, tx, hidden, is_player);
        return true;
    }
};
export constexpr auto TOKEN_SIZE_VALUES = std::array{0.5f, 1.f, 1.f, 2.f, 3.f, 4.f};

export struct AreaMarker {
    enum Shape : uint { SPHERE, BOX, CONE, LINE };

    Shape shape;
    float size;
    vec3  color;
    bool  hidden      = false;
    bool  block_light = false;

    using Container = ComponentVector<AreaMarker>;

    static bool serialize(AreaMarker const& data, xml::Element* entity) {
        auto* properties = entity->GetDocument()->NewElement("AreaMarker");
        entity->InsertEndChild(properties);

        xml::setAttribute(properties, "shape", uint(data.shape));
        xml::setAttribute(properties, "size", data.size);
        xml::setAttribute(properties, "color", data.color);
        xml::setAttribute(properties, "hidden", data.hidden);
        xml::setAttribute(properties, "block_light", data.block_light);
        return true;
    }

    static bool deserialize(Container* container, EntityID entity_id, xml::Element const* entity) {
        auto* properties = entity->FirstChildElement("AreaMarker");
        if(!properties) return false;

        uint  shape;
        float size;
        vec3  color;
        bool  hidden      = false;
        bool  block_light = false;
        xml::getAttribute(properties, "shape", shape);
        xml::getAttribute(properties, "size", size);
        xml::getAttribute(properties, "color", color);
        xml::getAttribute(properties, "hidden", hidden);
        xml::getAttribute(properties, "block_light", block_light);

        container->assignComponent(entity_id, Shape(shape), size, color, hidden, block_light);
        return true;
    }
};

export struct Sprite {
    Asset<Texture> texture;
    vec2           size;
    float          rotation = 0;

    using Container = ComponentVector<Sprite>;

    static bool deserialize(Container* container, EntityID entity_id, xml::Element const* entity) {
        auto* properties = entity->FirstChildElement("Sprite");
        if(!properties) return false;

        std::string tex;
        if(!xml::getAttribute(properties, "texture", tex)) return false;

        Asset<Texture> texture{tex};
        if(!texture) {
            logInfo("Sprite texture '{}' has not yet been loaded. Loading it now ...", tex);
            texture.request(tex);
        }

        vec2 size = {1, 1};
        xml::getAttribute(properties, "width", size.x);
        xml::getAttribute(properties, "height", size.y);

        container->assignComponent(entity_id, texture, size);
        return true;
    }

    static bool serialize(Sprite const& data, xml::Element* entity) {
        auto* properties = entity->GetDocument()->NewElement("Sprite");
        entity->InsertEndChild(properties);

        std::string texture{data.texture.getAlias()};
        if(texture.empty()) return false;

        xml::setAttribute(properties, "texture", texture);
        xml::setAttribute(properties, "width", data.size.x);
        xml::setAttribute(properties, "height", data.size.y);
        return true;
    }
};

export struct Light {
    vec2 range;

    using Container = ComponentVector<Light>;

    static bool serialize(Light const& data, xml::Element* entity) {
        auto* properties = entity->GetDocument()->NewElement("Light");
        entity->InsertEndChild(properties);

        xml::setAttribute(properties, "range", data.range);
        return true;
    }

    static bool deserialize(Container* container, EntityID entity_id, xml::Element const* entity) {
        auto* properties = entity->FirstChildElement("Light");
        if(!properties) return false;

        vec2 range;
        xml::getAttribute(properties, "range", range);

        container->assignComponent(entity_id, range);
        return true;
    }
};

export struct Initiative {
    int  roll;
    int  bonus;
    bool active = false;

    using Container = ComponentVector<Initiative>;

    static bool serialize(Initiative const& data, xml::Element* entity) {
        auto* properties = entity->GetDocument()->NewElement("Initiative");
        entity->InsertEndChild(properties);

        xml::setAttribute(properties, "roll", data.roll);
        xml::setAttribute(properties, "bonus", data.bonus);
        return true;
    }

    static bool deserialize(Container* container, EntityID entity_id, xml::Element const* entity) {
        auto* properties = entity->FirstChildElement("Light");
        if(!properties) return false;

        int roll;
        int bonus;
        xml::getAttribute(properties, "roll", roll);
        xml::getAttribute(properties, "bonus", bonus);

        container->assignComponent(entity_id, roll, bonus);
        return false;
    }
};

} // namespace bembel::examples::gm_helper
