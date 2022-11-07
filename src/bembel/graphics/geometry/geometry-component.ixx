module;
#include "bembel/pch.h"
export module bembel.graphics.geometry:Components;

import bembel.base;
import bembel.kernel;

namespace bembel::graphics {
using namespace bembel::base;
using namespace bembel::kernel;

export class GeometryComponent {
  public:
    using RendererID = uint;

    AssetHandle m_model;
    RendererID  m_renderer;

    static constexpr std::string_view COMPONENT_TYPE_NAME = "GeometryComponent";

    static bool initComponent(
        In<xml::Element const*>  properties,
        InOut<AssetManager>      asset_mgr,
        InOut<GeometryComponent> component
    );

    using ContainerType = ComponentArray<GeometryComponent>;
};

} // namespace bembel::graphics
