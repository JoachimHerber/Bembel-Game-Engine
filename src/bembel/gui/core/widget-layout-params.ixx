module;
#include "bembel/pch.h"
export module bembel.gui.core:WidgetLayoutParams;

import bembel.base;
import bembel.kernel;

namespace bembel::gui {
using namespace bembel::base;
using namespace bembel::kernel;

export class WidgetLayoutParams {
  public:
    vec2  rel_pos{0, 0};
    ivec2 pos_offset{0, 0};
    vec2  rel_size{0, 0};
    ivec2 size_offset{0, 0};

    u16 min_width{0};
    u16 min_height{0};

    struct {
        u32 left{0};
        u32 right{0};
        u32 bottom{0};
        u32 top{0};
    } margin;

    void configure(xml::Element const* properties);
};

} // namespace bembel::gui