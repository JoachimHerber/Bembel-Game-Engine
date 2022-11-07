module;
#include "bembel/pch.h"
module bembel.gui.core;

import bembel.base;
import bembel.kernel;

namespace bembel::gui {
using namespace bembel::base;
using namespace bembel::kernel;

Factory<WidgetLayout>& WidgetLayout::GetLayouterFactory() {
    static Factory<WidgetLayout> factory{};
    return factory;
}

LinearWidgetLayout::~LinearWidgetLayout() {}

inline void stringToLower(std::string& str) {
    std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) {
        return std::tolower(c);
    });
}

bool LinearWidgetLayout::configure(In<xml::Element const*> properties) {
    static const std::unordered_map<std::string, Direction> string_to_direction_enum = {
        {"vertical",   Direction::VERTICAL  },
        {"horizontal", Direction::HORIZONTAL}
    };

    std::string attrib;
    if(xml::getAttribute(properties, "layout_direction", attrib)) {
        stringToLower(attrib);
        auto direction_it = string_to_direction_enum.find(attrib);
        if(direction_it != string_to_direction_enum.end()) m_direction = direction_it->second;
    }

    return true;
}

void LinearWidgetLayout::calculateLayout(In<ivec2> size, In<std::span<Widget* const>> widgets) {
    if(m_direction == Direction::HORIZONTAL)
        calculateHorizontalLayout(size, widgets);
    else
        calculateVerticalLayout(size, widgets);
}

void LinearWidgetLayout::calculateHorizontalLayout(
    In<ivec2> size, In<std::span<Widget* const>> widgets
) {
    float  min_width    = 0.f;
    float  sum_wights   = 0.f;
    size_t num_widdgets = 0;

    for(auto widget : widgets) {
        if(widget->isHidden()) continue;

        auto& params = widget->getLayoutParams();

        min_width += std::max<int>(widget->getMinSize().x, params.min_width);
        min_width += params.margin.left + params.margin.right;
        sum_wights += params.rel_size.x;
        ++num_widdgets;
    }

    float spacing = (m_primary_axis == PrimaryAxis::SPACE_EQUALLY)
                      ? (num_widdgets + 1.f) / (size.x - min_width)
                      : 0;

    float scale = (size.x - min_width) / sum_wights;

    float offset = 0.0;
    switch(m_primary_axis) {
        case PrimaryAxis::ALIGN_CENTER: offset = (size.x - min_width) * 0.5f; break;
        case PrimaryAxis::ALIGN_RIGHT: offset = size.x - min_width; break;
        case PrimaryAxis::SPACE_EQUALLY: offset = 0.5 * spacing; break;
    }

    for(auto widget : widgets) {
        if(widget->isHidden()) continue;

        auto& params = widget->getLayoutParams();

        ivec2 widget_size =
            glm::max(widget->getMinSize(), ivec2(params.min_width, params.min_height));
        if(m_primary_axis == PrimaryAxis::SCALE_TO_FIT) widget_size.x += params.rel_size.x * scale;
        if(m_secondary_axis == SecondaryAxis::SCALE_TO_FIT)
            widget_size.y = size.y - params.margin.top - params.margin.bottom;

        widget->size.set(widget_size);

        ivec2 widget_pos;
        widget_pos.x = params.margin.left + offset;
        switch(m_secondary_axis) {
            case SecondaryAxis::SCALE_TO_FIT:
            case SecondaryAxis::ALIGN_BUTTON: widget_pos.y = params.margin.bottom; break;
            case SecondaryAxis::ALIGN_CENTER:
                widget_pos.y =
                    0.5f * (size.y - params.margin.top - params.margin.bottom - widget_size.y);
                break;
            case SecondaryAxis::ALIGN_TOP:
                widget_pos.y = size.y - params.margin.top - widget_size.y;
                break;
        }
        widget->position.set(widget_pos);

        offset += widget_size.x + params.margin.left + params.margin.right + spacing;
    }
}

void LinearWidgetLayout::calculateVerticalLayout(
    In<ivec2> size, In<std::span<Widget* const>> widgets
) {
    float  min_height   = 0.f;
    float  sum_wights   = 0.f;
    size_t num_widdgets = 0;

    for(auto widget : widgets) {
        if(widget->isHidden()) continue;

        auto& params = widget->getLayoutParams();

        min_height += std::max<int>(widget->getMinSize().y, params.min_height);
        min_height += params.margin.bottom + params.margin.top;
        sum_wights += params.rel_size.y;
        ++num_widdgets;
    }

    float spacing = (m_primary_axis == PrimaryAxis::SPACE_EQUALLY)
                      ? (num_widdgets + 1.f) / (size.y - min_height)
                      : 0;

    float scale = (size.y - min_height) / sum_wights;

    float offset = 0.0;

    switch(m_primary_axis) {
        case PrimaryAxis::SCALE_TO_FIT: offset = size.y; break;
        case PrimaryAxis::ALIGN_CENTER: offset = 0.5 * (size.y - min_height); break;
        case PrimaryAxis::ALIGN_TOP: offset = size.y; break;
        case PrimaryAxis::ALIGN_BUTTON: offset = size.y - min_height; break;
        case PrimaryAxis::SPACE_EQUALLY: offset = size.y - 0.5f * spacing; break;
    }

    for(auto widget : widgets) {
        if(widget->isHidden()) continue;

        auto& params = widget->getLayoutParams();

        ivec2 widget_size =
            glm::max(widget->getMinSize(), ivec2(params.min_width, params.min_height));
        if(m_secondary_axis == SecondaryAxis::SCALE_TO_FIT)
            widget_size.x = size.x - params.margin.left - params.margin.right;
        if(m_primary_axis == PrimaryAxis::SCALE_TO_FIT) widget_size.y += params.rel_size.y * scale;

        widget->size.set(widget_size);

        ivec2 widget_pos;
        switch(m_secondary_axis) {
            case SecondaryAxis::SCALE_TO_FIT:
            case SecondaryAxis::ALIGN_LEFT: widget_pos.x = params.margin.left; break;
            case SecondaryAxis::ALIGN_CENTER:
                widget_pos.x =
                    0.5f * (size.x - params.margin.left - params.margin.right - widget_size.x);
                break;
            case LinearWidgetLayout::SecondaryAxis::ALIGN_RIGHT:
                widget_pos.x = size.x - params.margin.right - widget_size.x;
                break;
        }
        widget_pos.y = offset - params.margin.top - widget_size.y;

        widget->position.set(widget_pos);

        offset -= widget_size.y + params.margin.top + params.margin.bottom + spacing;
    }
}

glm::ivec2 LinearWidgetLayout::calculateMinSize(In<std::span<Widget const* const>> widgets) {
    ivec2 min_size = {0, 0};

    for(auto widget : widgets) {
        if(widget->isHidden()) continue;

        auto& params = widget->getLayoutParams();

        ivec2 min_widget_size =
            glm::max(widget->getMinSize(), ivec2(params.min_width, params.min_height));

        min_widget_size.x += params.margin.left + params.margin.right;
        min_widget_size.y += params.margin.bottom + params.margin.top;

        if(m_direction == Direction::VERTICAL) {
            min_size.x = std::max(min_size.x, min_widget_size.x);
            min_size.y += min_widget_size.y;
        } else {
            min_size.x += min_widget_size.x;
            min_size.y = std::max(min_size.y, min_widget_size.y);
        }
    }
    return min_size;
}

bool LinearWidgetLayout::registerd =
    WidgetLayout::GetLayouterFactory().registerObjectGenerator<LinearWidgetLayout>("linear");

bool RelativeWidgetLayout::configure(xml::Element const*) {
    return true;
}

void RelativeWidgetLayout::calculateLayout(
    In<ivec2> area_size, In<std::span<Widget* const>> widgets
) {
    vec2 scale = area_size;

    for(auto* widget : widgets) {
        if(widget->isHidden()) continue;
        auto& p = widget->getLayoutParams();

        vec2 pos  = scale * p.rel_pos + vec2(p.pos_offset);
        vec2 size = scale * p.rel_size + vec2(p.size_offset);

        size.x = std::max<int>(size.x, p.min_width);
        size.y = std::max<int>(size.y, p.min_height);

        size.x -= p.margin.left + p.margin.right;
        size.y -= p.margin.bottom + p.margin.top;
        pos.x += p.margin.left;
        pos.y += p.margin.bottom;

        widget->position.set(pos);
        widget->size.set(size);
    }
}

glm::ivec2 RelativeWidgetLayout::calculateMinSize(In<std::span<Widget const* const>> widgets) {
    glm::ivec2 minSize{1, 1};

    for(auto* widget : widgets) {
        if(widget->isHidden()) continue;
        auto& p = widget->getLayoutParams();

        vec2  rel_pos     = p.rel_pos;
        ivec2 pos_offset  = p.pos_offset;
        vec2  rel_size    = p.rel_size;
        ivec2 size_offset = p.size_offset;

        //                        0 < pos_offset + rel_pos*minSize
        // <=>  -pos_offset/rel_pos < minSize;
        if(pos_offset.x < 0) minSize.x = std::max<int>(minSize.x, -pos_offset.x / rel_pos.x);
        if(pos_offset.y < 0) minSize.y = std::max<int>(minSize.y, -pos_offset.y / rel_pos.y);
        //      pos_offset + size_offset + (rel_pos + rel_size)*minSize < minSize
        // <=>  pos_offset + size_offset                                < minSize - (rel_pos +
        // rel_size) * minSize
        // <=>  pos_offset + size_offset                                < (1 - rel_pos -
        // rel_size) * minSize
        // <=> (pos_offset + size_offset) /(1 - rel_pos - rel_size)     < minSize
        minSize.x =
            std::max<int>(minSize.x, (pos_offset.x + size_offset.x) / (1 - rel_pos.x - rel_size.x));
        minSize.y =
            std::max<int>(minSize.y, (pos_offset.y + size_offset.y) / (1 - rel_pos.y - rel_size.y));

        //      min_width                          < size_offset + rel_size*minSize
        // <=> (min_width - size_offset) /rel_size < minSize
        minSize.x = std::max<int>(minSize.x, (p.min_width - size_offset.x) / rel_size.x);

        //      min_height                          < size_offset + rel_size*minSize
        // <=> (min_height - size_offset) /rel_size < minSize
        minSize.y = std::max<int>(minSize.y, (p.min_height - size_offset.y) / rel_size.y);
    }
    return minSize;
}

bool RelativeWidgetLayout::registerd =
    WidgetLayout::GetLayouterFactory().registerObjectGenerator<RelativeWidgetLayout>("relative");
} // namespace bembel::gui