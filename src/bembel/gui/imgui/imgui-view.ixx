module;
#include "imgui.h"
export module bembel.gui.imgui:View;

import bembel.base;
import bembel.kernel;

namespace bembel::gui::imgui {
using namespace bembel::base;
using namespace bembel::kernel;

export class View : public kernel::Viewport::View {
  public:
    void draw(In<ivec2> viewport_position, In<uvec2> viewport_size);

  protected:
    virtual void drawUI(In<ivec2> viewport_position, In<uvec2> viewport_size) = 0;
};

} // namespace bembel::gui::imgui