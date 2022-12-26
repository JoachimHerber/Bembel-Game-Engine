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
    Renderer( Widget& root_widget);
    ~Renderer();

    bool init(In<xml::Element const*> properties);
    bool init(In<Asset<ShaderProgram>> shader, In<Asset<Style>> style);

    void drawGui(ivec2 viewport_position, uvec2 viewport_size);

    Style* getStyle();

  protected:
    void drawWidget(Widget* widget, ivec2 parent_pos, ivec2 area_min, ivec2 area_max, int layer);

  protected:
    Widget&       m_root_widget;

    RenderBatch m_batch;

    Asset<ShaderProgram> m_shader;
    Asset<Style>         m_style;
};

} // namespace bembel::gui
