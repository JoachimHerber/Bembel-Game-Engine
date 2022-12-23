module;
export module bembel.graphics.pipeline:LightSource;

import bembel.base;
import bembel.kernel;

namespace bembel::graphics {
using namespace bembel::base;
using namespace bembel::kernel;

export struct PointLightData {
    vec3 color;

    float bulb_radius;
    float cutoff_radius;
};

export struct DirectionalLightData {
    vec3 color;
    vec3 direction;
};

export bool initComponent(
    In<xml::Element const*> properties,
    InOut<AssetManager>     asset_mgr,
    InOut<PointLightData>   component
);

export bool initComponent(
    In<xml::Element const*>       properties,
    InOut<AssetManager>           asset_mgr,
    InOut<DirectionalLightData> component
);

export using PointLight       = BasicComponent<"PointLight", PointLightData, ComponentContainer::MAP>;
export using DirectionalLight = BasicComponent<"DirectionalLight", DirectionalLightData, ComponentContainer::MAP>;

} // namespace bembel::graphics
