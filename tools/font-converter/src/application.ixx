﻿module;
#include <string_view>
export module bembel.tools.font_converter;

import bembel;
import :FontConverter;
import :FontView;
import :FontFamily;
import :FontTextureGenerator;

namespace bembel::tools {
using namespace bembel::base;
using bembel::gui::ButtonWidget;
using bembel::gui::CheckBoxWidget;
using bembel::gui::GraphicalUserInterface;
using bembel::gui::GroupWidget;
using bembel::gui::GuiSystem;
using bembel::gui::IntSliderWidget;
using bembel::gui::LabelWidget;
using bembel::gui::LinearWidgetLayout;
using bembel::gui::RadioButtonGroupWidget;
using bembel::gui::TextInputWidget;
using bembel::kernel::FileDropEvent;
using bembel::kernel::Viewport;
using bembel::kernel::Window;
using bembel::kernel::WindowResizeEvent;
using bembel::kernel::WindowShouldCloseEvent;

export class Application : public kernel::Application<GuiSystem> {
  public:
    Application();
    ~Application();

    virtual bool init(std::span<std::string_view> args) override;
    virtual void cleanup() override;

    virtual void update(double time) override;

    void handleEvent(In<WindowShouldCloseEvent>);
    void handleEvent(In<WindowResizeEvent>);
    void handleEvent(In<FileDropEvent>);

  private:
    bool initUserInterface();

    void lodeFontFile();
    void onFontFilePathChanged(In<std::u8string>, In<std::u8string>);
    void onFontFamilyAdded(In<std::u8string_view> name);
    void onSelectFontFamily(int index);
    void onTextureResulutionUpdate(i64 res);
    void onConvertFont();
    void onSaveFont();


  private:
    std::unique_ptr<FontConverter>   m_converter;
    std::unique_ptr<FontView>        m_font_view;

    Window* m_main_window;

    GraphicalUserInterface* m_gui;

    LinearWidgetLayout* m_layout;

    struct Widgets {
        LabelWidget*     load_file_label;
        TextInputWidget* load_file_path_input;
        ButtonWidget*    load_file_button;
        LabelWidget*     load_file_error;

        LabelWidget*            font_selections_label;
        RadioButtonGroupWidget* font_family_selections;
        CheckBoxWidget*         type_face_selection[4];

        LabelWidget*     char_set_label;
        CheckBoxWidget*  char_set_selection[4];
        LabelWidget*     additional_chars_label;
        TextInputWidget* additional_chars_input;

        LabelWidget*     sdf_label;

        LabelWidget*     texture_size_label;
        IntSliderWidget* texture_size_slider;

        ButtonWidget* convert_font_button;
        ButtonWidget* save_font_button;
        LabelWidget*  save_file_error;

        void createWidgets(GraphicalUserInterface* gui);

        void initLayout(LinearWidgetLayout* layout);
    } m_widgets;
};

} // namespace bembel::tools
