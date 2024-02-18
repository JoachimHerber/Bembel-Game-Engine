module;
#include <array>
#include <string>
#include <string_view>
export module bembel.examples.gm_helper:Components;

import bembel;

namespace bembel::examples::gm_helper {
using namespace base;
using namespace kernel;
using namespace graphics;

export struct Selectable {
    float dist;
    bool  selected = false;

    using Container = ComponentVector<Selectable>;
};

export struct Token {
    enum Size : uint { Tiny, Small, Medium, Large, Huge, Gargantuan };
    enum Type : uint { Player, Monster, NPC };

    Type           type;
    Size           size;
    std::string    name;
    Asset<Texture> textrue;
    bool           hidden = false;

    using Container = ComponentVector<Token>;
};
export constexpr auto TOKEN_SIZE_VALUES = std::array{0.5f, 1.f, 1.f, 2.f, 3.f, 4.f};

export struct Sprite {
    Asset<Texture> textrue;
    vec2           size;
    float          rotation = 0;

    using Container = ComponentVector<Sprite>;

    static bool deserialize(Container* container, EntityID entity_id, xml::Element const* entity) {
        auto* properties = entity->FirstChildElement("Sprite");
        if(!properties) return false;

        std::string tex;
        if(!xml::getAttribute(properties, "texture", tex)) return false;

        Asset<Texture> texture{tex};

        vec2 size = {1, 1};
        xml::getAttribute(properties, "width", size.x);
        xml::getAttribute(properties, "height", size.y);

        container->assignComponent(entity_id, texture, size);
        return true;
    }
};

export struct Light {
    vec2 range;

    using Container = ComponentVector<Light>;
};

export struct LightBlocker {
    vec2 size;

    using Container = ComponentVector<LightBlocker>;
};

} // namespace bembel::examples::gm_helper
