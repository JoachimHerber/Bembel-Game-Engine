module;
#include "bembel/pch.h"
export module bembel.graphics.pipeline:LightSource;

import bembel.base;
import bembel.kernel;

namespace bembel::graphics {
using namespace bembel::base;
using namespace bembel::kernel;

export class PointLightSource {
  public:
    vec3 color;

    float bulb_radius;
    float cutoff_radius;

    static constexpr std::string_view COMPONENT_TYPE_NAME = "PointLight";

    static bool initComponent(
        In<xml::Element const*> properties,
        InOut<AssetManager>     asset_mgr,
        InOut<PointLightSource> component
    );

    using ContainerType = ComponentMap<PointLightSource>;
    using ContainerPtr  = std::shared_ptr<ContainerType>;
};

export class DirectionalLightSource {
  public:
    vec3 color;
    vec3 direction;

    static constexpr std::string_view COMPONENT_TYPE_NAME = "DirectionalLight";

    static bool initComponent(
        In<xml::Element const*>       properties,
        InOut<AssetManager>           asset_mgr,
        InOut<DirectionalLightSource> component
    );
    using ContainerType = ComponentMap<DirectionalLightSource>;
    using ContainerPtr  = std::shared_ptr<ContainerType>;
};

} // namespace bembel::graphics
