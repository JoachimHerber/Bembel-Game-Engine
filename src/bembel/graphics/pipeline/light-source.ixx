module;
#include <memory>
#include <string_view>
#include <vector>
export module bembel.graphics.pipeline:LightSource;

import bembel.base;
import bembel.kernel;

namespace bembel::graphics {
using namespace bembel::base;
using namespace bembel::kernel;

export struct PointLight {
    vec3  color;
    float bulb_radius;
    float cutoff_radius;

    //PointLight(vec3 _color, float _bulb_radius, float _cutoff_radius)
    //  : color(_color), bulb_radius(_bulb_radius), cutoff_radius(_cutoff_radius) {}

    using Container = ComponentMap<PointLight>;

    static bool deserialize(Container* container, EntityID entity_id, xml::Element const* entity);
};

export struct DirectionalLight {
    vec3 color;
    vec3 direction;
    bool cast_shadow;

    //DirectionalLight(vec3 _color, vec3 _direction, bool _cast_shadow)
    //  : color(_color), direction(_direction), cast_shadow(_cast_shadow) {}

    using Container = ComponentMap<DirectionalLight>;

    static bool deserialize(Container* container, EntityID entity_id, xml::Element const* entity);
};

} // namespace bembel::graphics
