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

    using Container = ComponentMap<PointLight>;

    static bool deserialize(Container* container, EntityID entity_id, xml::Element const* entity);
};

export struct DirectionalLight {
    vec3 color;
    vec3 direction;
    bool cast_shadow;

    using Container = ComponentMap<DirectionalLight>;

    static bool deserialize(Container* container, EntityID entity_id, xml::Element const* entity);
};

} // namespace bembel::graphics
