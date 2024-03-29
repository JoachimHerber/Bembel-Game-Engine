﻿module;
#include <algorithm>
#include <bit>
#include <cassert>
#include <cmath>
#include <memory>
#include <string>
module bembel.gui.widgets;

import bembel.base;
import bembel.kernel;
import bembel.gui.core;

namespace bembel::gui {
using namespace bembel::base;
using namespace bembel::kernel;

SliderWidget::SliderWidget(In<Widget*> parent) : Widget{parent} {
    m_interaction_handles.push_back(&m_handle);

    size.change_signal.bind(this, &SliderWidget::onSizeChanged);
    m_handle.press_signal.bind(this, &SliderWidget::onHandlePress);
    m_handle.release_signal.bind(this, &SliderWidget::onHandleRelease);
    m_handle.dragging_signal.bind(this, &SliderWidget::onHandleMoved);

    m_handle.cursor = Asset<CursorIcon>("Hand");

    m_view = std::make_unique<SliderWidgetView>(this);
}

SliderWidget::~SliderWidget() {}

bool SliderWidget::configure(base::xml::Element const* properties) {
    Widget::configure(properties);
    return true;
}

uint SliderWidget::getMinWidth(In<std::optional<uint>>) const {
    auto style = getStyle();
    assert(style && "GUI::Style is undefined");

    float border       = style->getValue(Style::Values::INPUT_BORDER_WIDTH);
    float handle_width = style->getValue(Style::Values::SLIDER_HANDLE_WIDTH);
    return 4 * border + handle_width;
}

uint SliderWidget::getMinHeight(In<std::optional<uint>>) const {
    auto style = getStyle();
    assert(style && "GUI::Style is undefined");

    float border = style->getValue(Style::Values::INPUT_BORDER_WIDTH);
    return 4 * border;
}

void SliderWidget::onSizeChanged(In<ivec2>, In<ivec2> new_size) {
    m_handle.size = new_size;
}

void SliderWidget::onHandlePress(In<ivec2> cursor) {
    m_active = true;
    updateSliderPos(cursor);
}

void SliderWidget::onHandleRelease(In<ivec2> cursor) {
    m_active = false;
}

void SliderWidget::onHandleMoved(In<ivec2> cursor, InOut<ivec2>) {
    updateSliderPos(cursor);
}

void SliderWidget::updateSliderPos(In<ivec2> cursor) {
    auto style = getStyle();
    assert(style && "GUI::Style is undefined");

    float border       = style->getValue(Style::Values::INPUT_BORDER_WIDTH);
    float handle_width = style->getValue(Style::Values::SLIDER_HANDLE_WIDTH);

    float slider_length = size.get().x - handle_width - 2 * border;

    double pos = double(cursor.x - border - 0.5 * handle_width) / slider_length;
    pos        = std::clamp(pos, 0.0, 1.0);
    position_constrain_signal.emit(pos);
    m_position = pos;
}

void SliderWidgetView::draw(InOut<RenderBatchInterface> batch) {
    auto style = m_slider->getStyle();
    assert(style && "GUI::Style is undefined");

    vec2 min = m_slider->position.get();
    vec2 max = min + vec2(m_slider->size.get());

    if(m_slider->isDisabled()) {
        batch.setColor(style->getColor(Style::Colors::INPUT_DISABLED));
    } else {
        batch.setColor(style->getColor(Style::Colors::INPUT));
    }
    batch.drawRectangle(min, max); //

    float border       = style->getValue(Style::Values::INPUT_BORDER_WIDTH);
    float handle_width = style->getValue(Style::Values::SLIDER_HANDLE_WIDTH);

    min += vec2(border);
    max -= vec2(border);

    float slider_length = max.x - min.x - handle_width;

    min.x += slider_length * m_slider->getSliderPosition();
    max.x = min.x + handle_width;

    if(m_slider->isDisabled()) {
        batch.setColor(style->getColor(Style::Colors::SLIDER_DISABLED));
    } else if(m_slider->isActive()) {
        batch.setColor(style->getColor(Style::Colors::SLIDER_ACTIVE));
    } else if(m_slider->isHovered()) {
        batch.setColor(style->getColor(Style::Colors::SLIDER_HOVERED));
    } else {
        batch.setColor(style->getColor(Style::Colors::SLIDER));
    }
    batch.drawRectangle(min, max);
}

IntSliderWidget::IntSliderWidget(In<Widget*> parent, In<i64> min, In<i64> max, In<bool> logarithmic)
  : SliderWidget{parent}
  , m_min{min}
  , m_max{max}
  , m_value{min}
  , m_logarithmic{logarithmic}
  , m_label{this} {
    m_child_widgets.push_back(&m_label);

    size.change_signal.bind(this, &IntSliderWidget::onSizeChanged);
    position_constrain_signal.bind(this, &IntSliderWidget::constrainSliderPos);

    updateLabel();

    m_label.setName("Label");
    m_label.setAlignment(LabelWidget::Alignment::Center);
}

bool IntSliderWidget::configure(xml::Element const* properties) {
    Widget::configure(properties);
    if(!properties) return true;

    xml::getAttribute(properties, "min", m_min);
    xml::getAttribute(properties, "max", m_max);
    i64 value = (m_min + m_max) / 2;
    xml::getAttribute(properties, "value", value);
    setValue(value);

    return true;
}

void IntSliderWidget::setValue(In<i64> value) {
    i64 old_value = m_value;
    if(m_logarithmic) {
        u64 bw = std::bit_width(u64(m_max / m_min));
        u64 i  = std::clamp<u64>(std::bit_width(u64(value / m_min)) - 1, 0, bw);

        m_value    = (1 << i) * m_min;
        m_position = i / double(bw);
    } else {
        m_value    = std::clamp(value, m_min, m_max);
        m_position = (m_value - m_min) / double(m_max - m_min);
    }
    if(m_value != old_value) {
        value_update_signal.emit(m_value);
        updateLabel();
    }
}

void IntSliderWidget::constrainSliderPos(InOut<double> pos) {
    i64 new_value = m_value;
    if(m_logarithmic) {
        auto bw = std::bit_width(u64(m_max / m_min));
        i64  i  = static_cast<i64>(std::round(pos * bw));
        pos     = i / double(bw);

        new_value = (1 << i) * m_min;
    } else {
        i64 i = static_cast<i64>(std::round(pos * (m_max - m_min)));
        if(m_step.has_value()) i -= i % m_step.value();
        pos = i / double(m_max - m_min);

        new_value = m_min + i;
    }
    if(new_value != m_value) {
        m_value = new_value;
        value_update_signal.emit(m_value);
        updateLabel();
    }
}

void IntSliderWidget::updateLabel() {
    m_label.setText(m_text(m_value));
}

FloatSliderWidget::FloatSliderWidget(In<Widget*> parent, In<float> min, In<float> max, In<bool> logarithmic)
  : SliderWidget{parent}
  , m_min{min}
  , m_max{max}
  , m_value{min}
  , m_logarithmic{logarithmic}
  , m_label{this} {
    m_child_widgets.push_back(&m_label);

    size.change_signal.bind(this, &FloatSliderWidget::onSizeChanged);
    position_constrain_signal.bind(this, &FloatSliderWidget::constrainSliderPos);

    updateLabel();

    m_label.setName("Label");
    m_label.setAlignment(LabelWidget::Alignment::Center);
}

bool FloatSliderWidget::configure(xml::Element const* properties) {
    Widget::configure(properties);
    if(!properties) return true;

    xml::getAttribute(properties, "min", m_min);
    xml::getAttribute(properties, "max", m_max);
    i64 value = (m_min + m_max) / 2;
    xml::getAttribute(properties, "value", value);
    setValue(value);

    return true;
}

void FloatSliderWidget::setValue(float value) {
    float old_value = m_value;
    if(m_logarithmic) {
        float bw  = std::log(m_max / m_min);
        float exp = std::clamp<u64>(std::log(value / m_min), 0, bw);

        m_value    = std::pow(1, exp) * m_min;
        m_position = exp / bw;
    } else {
        m_value    = std::clamp(value, m_min, m_max);
        m_position = (m_value - m_min) / double(m_max - m_min);
    }
    if(m_value != old_value) {
        value_update_signal.emit(m_value);
        updateLabel();
    }
}

void FloatSliderWidget::constrainSliderPos(InOut<double> pos) {
    float new_value = m_value;
    if(m_logarithmic) {
        auto  bw  = std::log(m_max / m_min);
        float exp = pos * bw;

        new_value = std::pow(1, exp) * m_min;
    } else {
        float i = pos * (m_max - m_min);
        if(m_step.has_value()) { i = std::round(i / (*m_step)) * (*m_step); }
        pos = i / double(m_max - m_min);

        new_value = m_min + i;
    }
    if(new_value != m_value) {
        m_value = new_value;
        value_update_signal.emit(m_value);
        updateLabel();
    }
}

void FloatSliderWidget::updateLabel() {
    m_label.setText(m_text(m_value));
}
} // namespace bembel::gui
