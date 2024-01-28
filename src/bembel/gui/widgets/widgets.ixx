module;
#include <string_view>
#include <filesystem>
export module bembel.gui.widgets;

export import :Label;
export import :Button;
export import :TextInput;
export import :Slider;
export import :Text;
export import :CheckBox;
export import :RadioButton;
export import :Window;
export import :Tabs;
export import :SplitGroup;
export import :Viewport;

import bembel.gui.core;

export namespace bembel::gui {
template <typename T>
void registerWidgetTypeInFactory() {
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
    registerWidgetTypeInFactory<RadioButtonGroupWidget>();
    registerWidgetTypeInFactory<WindowWidget>();
    registerWidgetTypeInFactory<TabGroupWidget>();
    registerWidgetTypeInFactory<SplitGroupWidget>();
    registerWidgetTypeInFactory<ViewportWidget>();
}

} // namespace bembel::gui