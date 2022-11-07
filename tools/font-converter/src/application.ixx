module;
#include "bembel/pch.h"
export module bembel.tools.font_converter;

import bembel;
import :FontConverter;
import :PreviewRenderer;
import :FontView;
import :FontFamily;

namespace bembel::tools {
using namespace bembel::base;
using bembel::gui::CheckBoxWidget;
using bembel::gui::GraphicalUserInterface;
using bembel::gui::GroupWidget;
using bembel::gui::GuiSystem;
using bembel::gui::LabeledButtonWidget;
using bembel::gui::LabelWidget;
using bembel::gui::RadioButtonGroupWidget;
using bembel::gui::TextInputWidget;
using bembel::kernel::FileDropEvent;
using bembel::kernel::Viewport;
using bembel::kernel::Window;
using bembel::kernel::WindowResizeEvent;
using bembel::kernel::WindowShouldCloseEvent;

export class Application : public kernel::Application {
  public:
    Application();
    ~Application();

    virtual bool init() override;
    virtual void cleanup() override;

    virtual void update(double time) override;

    void handleEvent(In<WindowShouldCloseEvent>);
    void handleEvent(In<WindowResizeEvent>);
    void handleEvent(In<FileDropEvent>);

  private:
    bool initUserInterface();

    void createWidgets();

    void lodeFontFile();
    void onFontFilePathChanged(In<std::u32string>, In<std::u32string>);
    void onFontFamilyAdded(In<std::string_view> name);
    void onSelectFontFamily(int index);

  private:
    GuiSystem*                       m_gui_system;
    std::unique_ptr<FontConverter>   m_converter;
    std::unique_ptr<PreviewRenderer> m_preview;
    std::unique_ptr<FontView>        m_font_view;

    Window* m_main_window;

    GraphicalUserInterface* m_gui;

    struct FilePathInput {
        LabelWidget*         label;
        GroupWidget*         group;
        TextInputWidget*     file_path;
        LabeledButtonWidget* button;
        LabelWidget*         error;

        void createWidgets(
            GraphicalUserInterface* gui,
            In<std::string_view>    label,
            In<std::string_view>    button_text
        );
    };

    struct FontSelection {
        RadioButtonGroupWidget* font_family;
        CheckBoxWidget*         check_boxes[4];

        void createWidgets(GraphicalUserInterface* gui);
    };

    struct {
        FilePathInput input;
        FontSelection font_selection;
        GroupWidget*  spacing;
        FilePathInput output;
    } m_widgets;
};

} // namespace bembel::tools
