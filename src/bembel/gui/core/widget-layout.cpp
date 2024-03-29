module;
#include <algorithm>
#include <cctype>
#include <string_view>
module bembel.gui.core;

import bembel.base;
import bembel.kernel;

namespace bembel::gui {
using namespace bembel::base;
using namespace bembel::kernel;

inline void stringToLower(std::string& str) {
    std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) {
        return std::tolower(c);
    });
}

LinearWidgetLayout::Mode getMode(
    In<xml::Element const*> properties, LinearWidgetLayout::Mode _default
) {
    std::string mode;
    xml::getAttribute(properties, "mode", mode);

    std::transform(mode.begin(), mode.end(), mode.begin(), [](unsigned char c) {
        return std::toupper(c);
    });

    if(mode == "SCALE_TO_FIT") return LinearWidgetLayout::Mode::SCALE_TO_FIT;
    if(mode == "ALIGN_BOTTON") return LinearWidgetLayout::Mode::ALIGN_BOTTON;
    if(mode == "ALIGN_LEFT") return LinearWidgetLayout::Mode::ALIGN_LEFT;
    if(mode == "ALIGN_CENTER") return LinearWidgetLayout::Mode::ALIGN_CENTER;
    if(mode == "ALIGN_RIGHT") return LinearWidgetLayout::Mode::ALIGN_RIGHT;
    if(mode == "ALIGN_TOP") return LinearWidgetLayout::Mode::ALIGN_TOP;
    if(mode == "SPACE_EQUALLY") return LinearWidgetLayout::Mode::SPACE_EQUALLY;

    return _default;
}

bool LinearWidgetLayout::configure(In<xml::Element const*> properties) {
    xml::getAttribute(properties, "left_margin", m_margin_left);
    xml::getAttribute(properties, "right_margin", m_margin_right);
    m_mode = getMode(properties, m_mode);

    for(auto row : xml::IterateChildElements(properties, "Row")) {
        auto mode       = getMode(row, Mode::SCALE_TO_FIT);
        auto height     = xml::getAttribute<uint>(row, "height");
        auto min_height = xml::getAttribute<uint>(row, "min_height");
        auto rel_height = xml::getAttribute<float>(row, "rel_height");

        addRow({mode, height, min_height.value_or(0), rel_height.value_or(0.f)});
        for(auto element : xml::IterateChildElements(row)) {
            std::string_view type = element->Value();
            if(type == "Spacing") {
                auto width = xml::getAttribute<uint>(element, "width");
                if(width) addSpacing(*width);
                continue;
            }

            auto child = m_group->createChildWidget(type, element);
            if(!child) return false;

            auto rel_width = xml::getAttribute<float>(element, "rel_width");
            auto min_width = xml::getAttribute<uint>(element, "min_width");
            addWidget(child, rel_width.value_or(1.f), min_width.value_or(0.f));
        }
    }
    return true;
}

uint LinearWidgetLayout::getMinWidth(In<std::optional<uint>>) const {
    uint min_width = 0;
    for(auto& row : m_rows) {
        uint min_row_width = row.margin_left + m_margin_left + m_margin_right;
        for(auto& element : row.elements) {
            uint element_width = std::max(element.min_width, element.widget->getMinWidth());
            min_row_width += element_width + element.margin_right;
        }
        min_width = std::max(min_width, min_row_width);
    }
    return min_width;
}

uint LinearWidgetLayout::getMinHeight(In<std::optional<uint>> width) const {
    uint min_height = 0;
    for(auto& row : m_rows) {
        uint min_row_height = 0;
        if(row.params.height.has_value()) {
            min_row_height = row.params.height.value();
        } else {
            min_row_height = row.params.min_height;
            for(auto& element : row.elements) {
                min_row_height = std::max(min_row_height, element.widget->getMinHeight());
            }
        }
        min_height += min_row_height;
    }
    return min_height;
}

void LinearWidgetLayout::updateLayout() {
    updateLayout(m_group->size.get());
}

void LinearWidgetLayout::updateLayout(In<vec2> size) {
    for(auto& row : m_rows) {
        row.min_width = row.margin_left + m_margin_left + m_margin_right;
        for(auto& element : row.elements) {
            element.width = std::max(element.min_width, element.widget->getMinWidth());
            row.min_width += element.width + element.margin_right;

            if(row.params.mode == Mode::SCALE_TO_FIT) {
                scaleElements(
                    row.elements, size.x - m_margin_left - m_margin_right - row.margin_left
                );
            }
        }
    }
    uint min_height = 0;
    for(auto& row : m_rows) {
        if(row.params.height.has_value()) {
            row.min_height = row.params.height.value();
        } else {
            row.min_height = row.params.min_height;
            for(auto& element : row.elements) {
                row.min_height =
                    std::max(row.min_height, element.widget->getMinHeight(element.width));
            }
        }
        min_height += row.min_height;
    }

    float height_Scale = 0;
    if(m_mode == Mode::SCALE_TO_FIT) {
        for(auto& row : m_rows) height_Scale += row.params.rel_height;
        height_Scale = (size.y - min_height) / height_Scale;
    }

    float y         = size.y;
    float y_spacing = 0;
    switch(m_mode) {
        case Mode::ALIGN_BOTTON: y -= /***/ (size.y - min_height); break;
        case Mode::ALIGN_CENTER: y -= 0.5 * (size.y - min_height); break;
        case Mode::SPACE_EQUALLY:
            y_spacing = (size.y - min_height) / float(m_rows.size());
            y -= 0.5 * y_spacing;
            break;
    }

    for(auto& row : m_rows) {
        uint height = row.min_height;
        if(m_mode == Mode::SCALE_TO_FIT) height += row.params.rel_height * height_Scale;

        y -= height;

        float x         = row.margin_left + m_margin_left;
        float x_spacing = 0;
        switch(row.params.mode) {
            case Mode::ALIGN_RIGHT: x += /***/ (size.x - row.min_width); break;
            case Mode::ALIGN_CENTER: x += 0.5 * (size.x - row.min_width); break;
            case Mode::SPACE_EQUALLY:
                x_spacing = (size.y - min_height) / float(m_rows.size());
                x += 0.5 * x_spacing;
                break;
        }

        for(auto& elem : row.elements) {
            elem.widget->size.set({elem.width, height});
            elem.widget->position.set({x, y});
            x += elem.width + elem.margin_right + x_spacing;
        }

        y -= y_spacing;
    }
}

void LinearWidgetLayout::scaleElements(std::span<Row::Element> elements, uint width) {
    float sum_rel_width = 0;

    std::vector<Row::Element*> unscaled_elements;
    for(auto& elem : elements) {
        width -= elem.margin_right;

        if(elem.rel_width > 0) {
            sum_rel_width += elem.rel_width;
            unscaled_elements.push_back(&elem);
        } else {
            width -= elem.width;
        }
    }
    std::vector<Row::Element*> tmp;
    float                      scale = width / sum_rel_width;
    while(unscaled_elements.size() != tmp.size()) {
        scale         = width / sum_rel_width;
        sum_rel_width = 0;
        tmp.clear();
        for(auto elem : unscaled_elements) {
            uint scaled_width = elem->rel_width * scale;
            if(scaled_width > elem->width) {
                sum_rel_width += elem->rel_width;
                tmp.push_back(elem);
            } else {
                width -= elem->width;
            }
        }
        std::swap(unscaled_elements, tmp);
    }
    for(auto elem : unscaled_elements) { elem->width = elem->rel_width * scale; }
}

LinearWidgetLayout& LinearWidgetLayout::addSpacing(In<uint> margin) {
    if(!m_rows.empty()) {
        if(!m_rows.back().elements.empty()) {
            m_rows.back().elements.back().margin_right += margin;
        } else {
            m_rows.back().margin_left += margin;
        }
    }
    return *this;
}

LinearWidgetLayout& LinearWidgetLayout::addRow(In<RowParams> params) {
    m_rows.emplace_back(params);
    return *this;
}

LinearWidgetLayout& LinearWidgetLayout::addWidget(
    In<Widget*> widget, In<float> rel_width, In<uint> min_width
) {
    if(!m_rows.empty()) { m_rows.back().elements.emplace_back(widget, rel_width, min_width); }
    return *this;
}

/*
bool LinearWidgetLayout::registerd =
    WidgetLayout::GetLayouterFactory().registerObjectGenerator<LinearWidgetLayout>("linear");
    //*/

bool RelativeWidgetLayout::configure(xml::Element const* properties) {
    for(auto it : xml::IterateChildElements(properties)) {
        auto child = m_group->createChildWidget(it->Value(), it);
        if(!child) return false;
        vec2 position        = {0.0f, 0.0f};
        vec2 size            = {1.0f, 1.0f};
        vec2 position_offset = {0.f, 0.f};
        vec2 size_offset     = {0.f, 0.f};
        xml::getAttribute(it, "position", position);
        xml::getAttribute(it, "size", size);
        xml::getAttribute(it, "position_offset", position_offset);
        xml::getAttribute(it, "size_offset", size_offset);
        addWidget(child, position, size, position_offset, size_offset);
    }
    return true;
}
uint RelativeWidgetLayout::getMinWidth(In<std::optional<uint>>) const {
    uint min_width = 1;

    for(auto& elem : m_elements) {
        if(elem.widget->isHidden()) continue;

        float rel_pos     = elem.rel_pos.x;
        float pos_offset  = elem.pos_offset.x;
        float rel_size    = elem.rel_size.x;
        float size_offset = elem.size_offset.x;

        // clang-format off
        //                        0 < pos_offset + rel_pos*min_width
        // <=>  -pos_offset/rel_pos < min_width;
        if(pos_offset < 0) min_width = std::max<uint>(min_width, -pos_offset / rel_pos);
        //      pos_offset + size_offset + (rel_pos + rel_size)*min_width < min_width
        // <=>  pos_offset + size_offset                                  < min_width - (rel_pos +  rel_size) * min_width
        // <=>  pos_offset + size_offset                                  < (1 - rel_pos - rel_size) * min_width
        // <=> (pos_offset + size_offset) /(1 - rel_pos - rel_size)       < min_width
        min_width = std::max<uint>(min_width, (pos_offset + size_offset) / (1 - rel_pos - rel_size));
        //      widget.min_width                          < size_offset + rel_size*min_width
        // <=> (widget.min_width - size_offset) /rel_size < min_width
        min_width = std::max<uint>(min_width, (elem.widget->getMinWidth() - size_offset) / rel_size);
        // clang-format on
    }
    return min_width;
}

uint RelativeWidgetLayout::getMinHeight(In<std::optional<uint>>) const {
    uint min_height = 1;

    for(auto& elem : m_elements) {
        if(elem.widget->isHidden()) continue;

        float rel_pos     = elem.rel_pos.y;
        float pos_offset  = elem.pos_offset.y;
        float rel_size    = elem.rel_size.y;
        float size_offset = elem.size_offset.y;

        // clang-format off
        //                        0 < pos_offset + rel_pos*min_height
        // <=>  -pos_offset/rel_pos < min_width;
        if(pos_offset < 0) min_height = std::max<uint>(min_height, -pos_offset / rel_pos);
        //      pos_offset + size_offset + (rel_pos + rel_size)*min_height < min_height
        // <=>  pos_offset + size_offset                                   < min_height - (rel_pos +  rel_size) * min_height
        // <=>  pos_offset + size_offset                                   < (1 - rel_pos - rel_size) * min_width
        // <=> (pos_offset + size_offset) /(1 - rel_pos - rel_size)        < min_width
        min_height = std::max<uint>(min_height, (pos_offset + size_offset) / (1 - rel_pos - rel_size));
        //      widget.min_height                          < size_offset + rel_size*min_height
        // <=> (widget.min_height - size_offset) /rel_size < min_height
        min_height = std::max<uint>(min_height, (elem.widget->getMinHeight() - size_offset) / rel_size);
        // clang-format on
    }
    return min_height;
}

void RelativeWidgetLayout::updateLayout() {
    updateLayout(m_group->size.get());
}

void RelativeWidgetLayout::updateLayout(In<vec2> size) {
    for(auto& elem : m_elements) {
        if(elem.widget->isHidden()) continue;

        vec2 elem_pos  = size * elem.rel_pos + elem.pos_offset;
        vec2 elem_size = size * elem.rel_size + elem.size_offset;

        elem_size.x = std::max<int>(elem_size.x, elem.widget->getMinWidth());
        elem_size.y = std::max<int>(elem_size.y, elem.widget->getMinHeight());

        // size.x -= p.margin.left + p.margin.right;
        // size.y -= p.margin.bottom + p.margin.top;
        // pos.x += p.margin.left;
        // pos.y += p.margin.bottom;

        elem.widget->position.set(elem_pos);
        elem.widget->size.set(elem_size);
    }
}

RelativeWidgetLayout& RelativeWidgetLayout::addWidget(
    In<Widget*> widget,
    In<vec2>    position,
    In<vec2>    size,
    In<vec2>    position_offset,
    In<vec2>    size_offset
) {
    m_elements.emplace_back(widget, position, size, position_offset, size_offset);
    return *this;
}

/*
bool RelativeWidgetLayout::registerd =
    WidgetLayout::GetLayouterFactory().registerObjectGenerator<RelativeWidgetLayout>("relative");

//*/
} // namespace bembel::gui
