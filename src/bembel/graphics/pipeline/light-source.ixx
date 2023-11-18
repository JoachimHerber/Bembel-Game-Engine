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
};

export struct DirectionalLight {
    vec3 color;
    vec3 direction;
    bool cast_shadow = true;
};

export bool initComponent(In<xml::Element const*> properties, InOut<PointLight> component);

export bool initComponent(In<xml::Element const*> properties, InOut<DirectionalLight> component);

} // namespace bembel::graphics

export template <>
struct bembel::kernel::ComponentMetaData<bembel::graphics::PointLight>
  : BasicComponentMetaData<
        "PointLight",
        bembel::graphics::PointLight,
        bembel::kernel::ComponentContainer::MAP> {};
export template <>
struct bembel::kernel::ComponentMetaData<bembel::graphics::DirectionalLight>
  : BasicComponentMetaData<
        "DirectionalLight",
        bembel::graphics::DirectionalLight,
        bembel::kernel::ComponentContainer::MAP> {};
