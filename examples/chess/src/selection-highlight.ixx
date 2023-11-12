module;
#include <chrono>
export module bembel.examples.chess:SelectionHighlight;

import bembel;

namespace bembel::examples::chess {
using namespace base;
using namespace kernel;
using namespace graphics;
using namespace gui;

export enum class SelectionHighlight { NO_HIGHLIGHT, SELECTABLE, FOCUSED, SELECTED };
export bool initComponent(xml::Element const*, SelectionHighlight&) {
    return true;
}
export using SelectionHighlightComponent = BasicComponent<"SelectionHighlight", SelectionHighlight>;

export class SelectionRenderingStage : public RenderingPipeline::Stage {
  public:
    SelectionRenderingStage(RenderingPipeline& pipline);
    ~SelectionRenderingStage();

    void setShader(Asset<ShaderProgram> const&);
    void setShader(Asset<ShaderProgram>&&);

    void setDepthOutputTexture(std::string_view);
    void setColorOutputTexture(std::string_view);

    virtual bool configure(xml::Element const*) override;

    virtual void init() override;
    virtual void cleanup() override;

    virtual void setScene(Scene*) override;
    virtual void execute(In<std::span<const RendererPtr>> renderer) override;

  private:
    struct GeometryObject {
        vec3           position;
        quat           rotation;
        float          dist;
        GeometryModel* model;
        unsigned       state;
    };
    void getHiglightedObjects(std::vector<GeometryObject>&);

  private:
    Asset<ShaderProgram> m_shader_program;

    std::unique_ptr<Texture> m_noise;

    Scene*                                  m_scene;
    Transform::Container*                   m_transform_components;
    Geometry::Container*                    m_geometry_components;
    SelectionHighlightComponent::Container* m_highlight_components;

    std::chrono::time_point<std::chrono::steady_clock> m_start_time;
};

} // namespace bembel::examples::chess
