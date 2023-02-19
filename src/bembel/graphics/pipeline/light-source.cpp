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

bool initComponent(In<xml::Element const*> properties, InOut<PointLightData> component) {
    xml::getAttribute(properties, "color", component.color);
    float intensity;
    if(xml::getAttribute(properties, "intensity", intensity)) component.color *= intensity;

    xml::getAttribute(properties, "bulbRadius", component.bulb_radius);
    xml::getAttribute(properties, "cutoffRadius", component.cutoff_radius);

    return true;
}

ShadowMap::ShadowMap(uint resolution, uint cascadeds)
  : resolution{resolution}
  , texture{Texture::Target::TEXTURE_2D, Texture::Format::DEPTH_COMPONENT24} {

    assert(0 < cascadeds && cascadeds <= MAX_CASCADEDS);
    //world_to_light_space.resize(cascadeds);

    texture.init(
        uvec2(resolution),
        Texture::MinFilter::NEAREST,
        Texture::MagFilter::NEAREST,
        Texture::Wrap::REPEAT,
        Texture::Wrap::REPEAT
    );
    fbo.setDepthAttechment(&texture);
    fbo.init();
}

bool initComponent(In<xml::Element const*> properties, InOut<DirectionalLightData> component) {
    xml::getAttribute(properties, "color", component.color);
    float intensity;
    if(xml::getAttribute(properties, "intensity", intensity)) component.color *= intensity;
    xml::getAttribute(properties, "direction", component.direction);
    component.direction = glm::normalize(component.direction);

    uint shadow_resolution = 0;
    if(xml::getAttribute(properties, "shadowResolution", shadow_resolution)) {
        component.shadow_map = std::make_unique<ShadowMap>(shadow_resolution);
    }

    return true;
}

} // namespace bembel::graphics
