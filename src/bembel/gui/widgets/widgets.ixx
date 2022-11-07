module;
#include "bembel/pch.h"
export module bembel.gui.widgets;

export import :Button;
export import :Label;
export import :LabeledButton;
export import :TextInput;
export import :Text;
export import :CheckBox;
export import :RadioButton;
export import :Window;

import bembel.gui.core;

export namespace bembel::gui {
template <typename T>
void registerWidgetTypeInFactory() {
    Widget::getFactory().registerObjectGenerator<T>(T::WIDGET_TYPE_NAME);
}

void registerWidgetTypesInFactory() {
    registerWidgetTypeInFactory<ButtonWidget>();
    registerWidgetTypeInFactory<LabelWidget>();
    registerWidgetTypeInFactory<LabeledButtonWidget>();
    registerWidgetTypeInFactory<TextWidget>();
    registerWidgetTypeInFactory<TextInputWidget>();
    registerWidgetTypeInFactory<CheckBoxWidget>();
    registerWidgetTypeInFactory<WindowWidget>();
}

} // namespace bembel::gui