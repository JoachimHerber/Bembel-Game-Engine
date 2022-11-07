module;
#include "bembel/pch.h"
module bembel.gui.core;

import bembel.base;
import bembel.kernel;
import bembel.gui.core;

namespace bembel::gui {
using namespace bembel::base;
using namespace bembel::kernel;

void WidgetLayoutParams::configure(xml::Element const* properties) {
    if(!properties) return;

    // clang-format off
    xml::getAttribute(properties, "rel_pos",     rel_pos     );
    xml::getAttribute(properties, "pos",         pos_offset  ) || xml::getAttribute(properties, "pos_offset",  pos_offset  );
    xml::getAttribute(properties, "rel_size",    rel_size    );
    xml::getAttribute(properties, "size",        size_offset ) || xml::getAttribute(properties, "size_offset", size_offset );
    xml::getAttribute(properties, "min_width",   min_width   );
    xml::getAttribute(properties, "min_height",  min_height  );
    
    if(xml::getAttribute(properties, "margin", margin.top)){
        margin.bottom = margin.right = margin.left = margin.top;
    }

    xml::getAttribute(properties, "margin_top",    margin.top    );
    xml::getAttribute(properties, "margin_bottom", margin.bottom );
    xml::getAttribute(properties, "margin_right",  margin.right  );
    xml::getAttribute(properties, "margin_left",   margin.left   );

    // clang-format on
}

} // namespace bembel::gui
