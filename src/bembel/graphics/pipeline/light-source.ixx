module;
#include "bembel/pch.h"
export module bembel.graphics.pipeline:LightSource;

import bembel.base;
import bembel.kernel;

namespace bembel::graphics {
using namespace bembel::base;
using namespace bembel::kernel;

export struct PointLightSource {
    vec3 color;

    float bulb_radius;
    float cutoff_radius;
};

export struct DirectionalLightSource {
    vec3 color;
    vec3 direction;
};

export bool initComponent(
    In<xml::Element const*> properties,
    InOut<AssetManager>     asset_mgr,
    InOut<PointLightSource> component
);

export bool initComponent(
    In<xml::Element const*>       properties,
    InOut<AssetManager>           asset_mgr,
    InOut<DirectionalLightSource> component
);

export using PointLightComponent = StandardComponent<"PointLight", PointLightSource, false>;
export using DirectionalLightComponent =
    StandardComponent<"DirectionalLight", DirectionalLightSource, false>;

} // namespace bembel::graphics
