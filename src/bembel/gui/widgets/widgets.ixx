module;
#include "bembel/pch.h"
export module bembel.gui.widgets;

export import :Label;
export import :Button;
export import :TextInput;
export import :Slider;
export import :Text;
export import :CheckBox;
export import :RadioButton;
export import :Window;

import bembel.gui.core;

export namespace bembel::gui {
template <typename T> void registerWidgetTypeInFactory() {
    Widget::getFactory().registerObjectGenerator<T>(T::WIDGET_TYPE_NAME);
}

void registerWidgetTypesInFactory() {
    registerWidgetTypeInFactory<ButtonWidget>();
    registerWidgetTypeInFactory<LabelWidget>();
    registerWidgetTypeInFactory<TextWidget>();
    registerWidgetTypeInFactory<TextInputWidget>();
    registerWidgetTypeInFactory<SliderWidget>();
    registerWidgetTypeInFactory<IntSliderWidget>();
    registerWidgetTypeInFactory<CheckBoxWidget>();
    registerWidgetTypeInFactory<WindowWidget>();
}

} // namespace bembel::gui