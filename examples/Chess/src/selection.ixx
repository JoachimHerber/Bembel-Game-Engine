module;
#include <chrono>
export module bembel.examples.chess.selection;

import bembel;

namespace bembel::examples::chess {
using namespace base;
using namespace kernel;
using namespace graphics;
using namespace gui;

export class SelectionPointer {
  public:
    SelectionPointer(RenderingPipeline*);
    ~SelectionPointer();

    void handleEvent(CursorMovedEvent const& event);
    void handleEvent(MouseButtonPressEvent const& event);

    vec3 const&  getRayOrigin() const { return m_ray_origin; }
    vec3 const&  getRayDirection() const { return m_ray_direction; }
    ivec2 const& getSelectedTile() const { return m_selected_tile; }

    Signal<>& getSelectSignal() { return m_select; }

  private:
    void updateRay(vec2 pos);

  private:
    RenderingPipeline* m_pipline;

    vec3     m_ray_origin;
    vec3     m_ray_direction;
    ivec2    m_selected_tile;
    Signal<> m_select;
};

export enum class SelectionHighlight { NO_HIGHLIGHT, SELECTABLE, FOCUSED, SELECTED };
export bool initComponent(xml::Element const*, AssetManager&, SelectionHighlight&) {
    return true;
}
export using SelectionHighlightComponent =
    BasicComponent<"SelectionHighlight", SelectionHighlight>;

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
    virtual void execute(GeometryRenderQueue& renderQueue, std::vector<RendererPtr> const& renderer)
        override;

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
