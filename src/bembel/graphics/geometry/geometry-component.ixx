module;
export module bembel.graphics.geometry:Components;

import bembel.base;
import bembel.kernel;
import :Model;

namespace bembel::graphics {
using namespace bembel::base;
using namespace bembel::kernel;

export struct Geometry {
    using RendererID = uint;

    Asset<GeometryModel> model;
    vec3                 scale       = {1, 1, 1};
    RendererID           renderer    = 0;
    bool                 cast_shadow = true;

    using Container = ComponentVector<bembel::graphics::Geometry>;

    static bool deserialize(Container* container, EntityID entity_id, xml::Element const* entity);
};

} // namespace bembel::graphics
