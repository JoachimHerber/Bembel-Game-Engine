#ifndef BEMBEL_GUI_RENDERER_GUIRENDERER_HPP
#define BEMBEL_GUI_RENDERER_GUIRENDERER_HPP

#include <bembel/base/utils/factory.hpp>
#include <bembel/kernel/assets/asset-manager.hpp>
#include <bembel/kernel/core/system.hpp>
#include <bembel/kernel/events/display-events.hpp>
#include <bembel/kernel/rendering/shader.hpp>

#include "../widgets/widget.hpp"
#include "./render-batch.hpp"
#include "./style.hpp"

namespace bembel::gui {
class GraphicalUserInterface;
class WidgetView;

class BEMBEL_API Renderer {
  public:
    Renderer(GraphicalUserInterface& gui);
    ~Renderer();

    bool init(const base::xml::Element* properties);

    void drawGui(const glm::ivec2& viewport_position, const glm::uvec2& viewport_size);

    Style* getStyle();

  protected:
    void drawWidget(
        Widget*           widget,
        const glm::ivec2& parent_pos,
        const glm::ivec2& area_min,
        const glm::ivec2& area_max,
        int               layer);

  protected:
    GraphicalUserInterface& gui;

    RenderBatch batch;

    kernel::Asset<kernel::ShaderProgram> shader;
    kernel::Asset<Style>                 style;
};

} // namespace bembel::gui
#endif // include guards
