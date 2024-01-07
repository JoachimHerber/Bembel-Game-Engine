module;
#include <string>
export module bembel.gui.widgets:Viewport;

import bembel.base;
import bembel.kernel;
import bembel.graphics;
import bembel.gui.core;

namespace bembel::gui {
using namespace bembel::base;
using namespace bembel::kernel;
using namespace bembel::graphics;

export class ViewportWidget : public Widget {
  public:
    static constexpr std::string_view WIDGET_TYPE_NAME = "Viewport";

  public:
    ViewportWidget(Widget& parent, Viewport* viewport = nullptr, Camera* camera = nullptr);
    ~ViewportWidget();

    virtual bool configure(xml::Element const* properties) override;

    virtual uint getMinWidth(In<std::optional<uint>>) const override;
    virtual uint getMinHeight(In<std::optional<uint>>) const override;

    virtual std::string_view getWidgetTypeName() const override { return WIDGET_TYPE_NAME; }

  protected:
    void onSizeOrPositionChanged(In<ivec2>, In<ivec2>);
    void onHandleMoved(In<ivec2> cursor, InOut<ivec2> movement);

  private:
    Viewport*         m_viewport;
    Camera*           m_camera;
    InteractionHandle m_handle;

    float             m_camera_pitch = -0.1f;
    float             m_camera_yaw   = 0;
    float             m_camera_dist  = 3.0f;
};

} // namespace bembel::gui
