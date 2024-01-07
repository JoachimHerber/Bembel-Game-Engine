module;
export module bembel.gui.core:Renderer;

import bembel.base;
import bembel.kernel;

import :Style;
import :Widget;

namespace bembel::gui {
using namespace bembel::base;
using namespace bembel::kernel;

export class Renderer {
  public:
    Renderer(Widget& root_widget);
    ~Renderer();

    bool init(In<xml::Element const*> properties);
    bool init(In<Asset<ShaderProgram>> shader, In<Asset<Style>> style);

    void drawGui(In<ivec2> viewport_position, In<uvec2> viewport_size);

    Style* getStyle();

  protected:
    void drawWidgets(
        In<Widget> widget, In<ivec2> parent_pos, In<ivec2> area_min, In<ivec2> area_max
    );
  protected:
    Widget& m_root_widget;

    RenderBatch m_batch;

    Asset<ShaderProgram> m_shader;
    Asset<Style>         m_style;
};

} // namespace bembel::gui
