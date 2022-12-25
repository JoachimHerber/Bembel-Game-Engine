module;
#include <glm/glm.hpp>
#include <memory>
#include <string_view>
module bembel.graphics.pipeline;

import bembel.base;
import bembel.kernel;

namespace bembel::graphics {
using namespace bembel::base;
using namespace bembel::kernel;

bool initComponent(
    In<xml::Element const*> properties,
    InOut<AssetManager>     asset_mgr,
    InOut<PointLightData>   component
) {
    xml::getAttribute(properties, "color", component.color);
    float intensity;
    if(xml::getAttribute(properties, "intensity", intensity)) component.color *= intensity;

    xml::getAttribute(properties, "bulbRadius", component.bulb_radius);
    xml::getAttribute(properties, "cutoffRadius", component.cutoff_radius);

    return true;
}

bool initComponent(
    In<xml::Element const*>     properties,
    InOut<AssetManager>         asset_mgr,
    InOut<DirectionalLightData> component
) {
    xml::getAttribute(properties, "color", component.color);
    float intensity;
    if(xml::getAttribute(properties, "intensity", intensity)) component.color *= intensity;
    xml::getAttribute(properties, "direction", component.direction);
    component.direction = glm::normalize(component.direction);

    uint shadow_resolution = 0;
    if(xml::getAttribute(properties, "shadowResolution", shadow_resolution)) {
        component.shadow_map = std::make_unique<Texture>(
            Texture::Target::TEXTURE_2D, Texture::Format::DEPTH_COMPONENT32
        );
        component.shadow_map->init(
            uvec2(shadow_resolution),
            Texture::MinFilter::LINEAR,
            Texture::MagFilter::LINEAR,
            Texture::Wrap::CLAMP_TO_EDGE,
            Texture::Wrap::CLAMP_TO_EDGE
        );
        component.shadow_fbo = std::make_unique<FrameBufferObject>();
        component.shadow_fbo->setDepthAttechment(component.shadow_map.get());
        component.shadow_fbo->init();
    }

    return true;
}

} // namespace bembel::graphics
