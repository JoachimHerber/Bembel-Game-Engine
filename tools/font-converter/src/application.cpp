module;
#include "bembel/pch.h"
module bembel.tools.font_converter;

namespace bembel::tools {
using namespace bembel::base;
using namespace bembel::gui;
using namespace bembel::kernel;

Application::Application() : kernel::Application(), m_converter(std::make_unique<FontConverter>()) {
    m_gui_system = m_engine.addSystem<GuiSystem>();

    auto& eventMgr = m_engine.getEventManager();
    eventMgr.addHandler<WindowShouldCloseEvent>(this);
    eventMgr.addHandler<WindowResizeEvent>(this);
    eventMgr.addHandler<FileDropEvent>(this);

    auto escapeKey = m_engine.getInputManager().getKeyboard().getKey(Keyboard::ESCAPE);
    escapeKey->press_signal.bind<Application>(this, &Application::quit);

    m_converter->font_family_added_signal.bind(this, &Application::onFontFamilyAdded);
}

Application::~Application() {
    auto& eventMgr = m_engine.getEventManager();
    eventMgr.removeHandler<WindowShouldCloseEvent>(this);
    eventMgr.removeHandler<WindowResizeEvent>(this);
    eventMgr.removeHandler<FileDropEvent>(this);
}

bool Application::init() {
    auto& asset_locator = m_engine.getAssetManager().getAssetLocator();
    asset_locator.addGenericAssetDirectory("../assets");
    asset_locator.addGenericAssetDirectory("../assets/gui");
    asset_locator.addGenericAssetDirectory("../assets/shader");

    auto display_mode = std::make_shared<WindowDisplayMode>();
    display_mode->setWidth(1200);
    display_mode->setHeight(900);
    display_mode->setAspectRatio(4, 3);

    m_main_window = m_engine.getDisplayManager().createWindow();
    m_main_window->setBackgroundColor(vec3{0.05f, 0.05f, 0.10f});
    m_main_window->setDisplayMode(display_mode);

    m_main_window->createViewport(vec2{0.00f, 0.f}, vec2{0.75f, 1.f});
    m_main_window->createViewport(vec2{0.75f, 0.f}, vec2{0.25f, 1.f});

    m_main_window->open("Font Converter");

    if(!initUserInterface()) return false;

    m_main_window->getViewport(1)->addView(&m_gui->getView());

    m_font_view = std::make_unique<FontView>(m_engine.getAssetManager());
    m_main_window->getViewport(0)->addView(m_font_view.get());

    m_engine.initSystems();

    return true;
}

void Application::cleanup() {
    m_font_view.reset();

    m_engine.shutdownSystems();
    m_engine.getDisplayManager().closeOpenWindows();
}

void Application::update(double time) {}

void Application::handleEvent(In<WindowShouldCloseEvent> event) {
    quit();
}

void Application::handleEvent(In<WindowResizeEvent> event) {}

void Application::handleEvent(In<FileDropEvent> event) {
    for(auto& file_path : event.files) { m_converter->loade(file_path); }

    m_converter->parseGlypes();
}

bool Application::initUserInterface() {
    Asset<ShaderProgram> gui_shader;
    Asset<gui::Style>    gui_style;

    gui_shader.request(m_engine.getAssetManager(), "gui.shader-program");
    gui_style.request(m_engine.getAssetManager(), "dark_gui_style.xml");

    if(!gui_shader || !gui_style) return false;

    m_gui = m_gui_system->createGUI("main");
    m_gui->getRenderer().init(gui_shader, gui_style);

    createWidgets();
    return true;
}

void Application::createWidgets() {
    m_gui->getRootWidget().setLayout(std::make_shared<LinearWidgetLayout>(
        LinearWidgetLayout::Direction::VERTICAL,
        LinearWidgetLayout::PrimaryAxis::ALIGN_TOP,
        LinearWidgetLayout::SecondaryAxis::SCALE_TO_FIT
    ));

    m_widgets.input.createWidgets(m_gui, "Load Font File:", "Load");

    m_widgets.input.file_path->text = std::u32string{U"fonts/FreeSans.ttf"};
    m_widgets.input.file_path->text.change_signal.bind(this, &Application::onFontFilePathChanged);
    m_widgets.input.button->click_signal.bind(this, &Application::lodeFontFile);

    m_widgets.font_selection.createWidgets(m_gui);
    m_widgets.font_selection.font_family->selection_change_signal.bind(
        this, &Application::onSelectFontFamily
    );
}

void Application::lodeFontFile() {
    std::filesystem::path path = m_widgets.input.file_path->text.get();
    if(!std::filesystem::exists(path)) {
        m_widgets.input.error->text.set("File dosn't exist");
        return;
    }
    if(!m_converter->loade(path)) {
        m_widgets.input.error->text.set("Failed to load file.");
        return;
    }

    m_converter->parseGlypes();
}

void Application::onFontFilePathChanged(In<std::u32string>, In<std::u32string>) {
    m_widgets.input.error->text.set("");
}

void Application::onFontFamilyAdded(In<std::string_view> name) {
    m_widgets.font_selection.font_family->addRadioButton(name);
    if(m_widgets.font_selection.font_family->getSelection() == -1)
        m_widgets.font_selection.font_family->setSelection(0);

    m_gui->getRootWidget().updateLayout();
}

void Application::onSelectFontFamily(int index) {
    auto font_family = m_converter->getFontFamily(index);
    if(!font_family) { return; }
    for(int i = 0; i < 4; ++i) {
        m_widgets.font_selection.check_boxes[i]->show();
        if(font_family->hasFace(FontFamily::FaceType(i))) {
            m_widgets.font_selection.check_boxes[i]->enable();
        } else {
            m_widgets.font_selection.check_boxes[i]->disable();
        }
    }

    m_gui->getRootWidget().updateLayout();
}

void Application::FilePathInput::createWidgets(
    GraphicalUserInterface* gui, In<std::string_view> label_text, In<std::string_view> button_text
) {
    label     = gui->getRootWidget().createChildWidget<LabelWidget>();
    group     = gui->getRootWidget().createChildWidget<GroupWidget>();
    file_path = group->createChildWidget<TextInputWidget>();
    button    = group->createChildWidget<LabeledButtonWidget>();
    error     = gui->getRootWidget().createChildWidget<LabelWidget>();

    label->text      = std::string(label_text);
    label->alignment = LabelWidget::Alignment::Left;
    label->outline   = true;

    button->text      = std::string(button_text);
    button->alignment = LabelWidget::Alignment::Center;
    button->outline   = true;

    error->text_color = {255, 0, 0, 255};
    error->alignment  = LabelWidget::Alignment::Left;
    error->outline    = true;

    group->setLayout(std::make_shared<LinearWidgetLayout>(
        LinearWidgetLayout::Direction::HORIZONTAL,
        LinearWidgetLayout::PrimaryAxis::SCALE_TO_FIT,
        LinearWidgetLayout::SecondaryAxis::SCALE_TO_FIT
    ));

    // clang-format off
    group->setLayoutParams({                                              .min_height = 32                                                            });
    label->setLayoutParams({                                              .min_height = 28, .margin = {.left = 6, .right = 6, .bottom = 0, .top = 16} });
    file_path->setLayoutParams({.rel_size = {1.f, 1.f },                  .min_height = 32, .margin = {.left = 4, .right = 2}                         });
    button->setLayoutParams({   .rel_size = {0.f, 0.f }, .min_width = 64, .min_height = 32, .margin = {.left = 2, .right = 4}                         });
    error->setLayoutParams({    .rel_size = {1.f, 1.f },                  .min_height = 24, .margin = {.left = 6, .right = 4, .bottom = 8, .top = 2}  });
    // clang-format on
}

void Application::FontSelection::createWidgets(GraphicalUserInterface* gui) {
    font_family = gui->getRootWidget().createChildWidget<RadioButtonGroupWidget>();

    auto group_0 = gui->getRootWidget().createChildWidget<GroupWidget>();
    auto group_1 = gui->getRootWidget().createChildWidget<GroupWidget>();

    check_boxes[0] = group_0->createChildWidget<CheckBoxWidget>();
    check_boxes[1] = group_0->createChildWidget<CheckBoxWidget>();
    check_boxes[2] = group_1->createChildWidget<CheckBoxWidget>();
    check_boxes[3] = group_1->createChildWidget<CheckBoxWidget>();

    check_boxes[0]->text = std::string{"Default"};
    check_boxes[1]->text = std::string{"Oblique"};
    check_boxes[2]->text = std::string{"Bold"};
    check_boxes[3]->text = std::string{"Bold & Oblique"};

    check_boxes[0]->hide();
    check_boxes[1]->hide();
    check_boxes[2]->hide();
    check_boxes[3]->hide();

    // clang-format off
    font_family->setLayoutParams({ .margin = {.left = 8, .right = 8} });

    group_0->setLayoutParams({ .margin = {.left = 8, .right = 8} });
    group_1->setLayoutParams({ .margin = {.left = 8, .right = 8} });

    group_0->setLayout(std::make_shared<LinearWidgetLayout>(
        LinearWidgetLayout::Direction::HORIZONTAL,
        LinearWidgetLayout::PrimaryAxis::SCALE_TO_FIT,
        LinearWidgetLayout::SecondaryAxis::SCALE_TO_FIT
    ));
    group_1->setLayout(std::make_shared<LinearWidgetLayout>(
        LinearWidgetLayout::Direction::HORIZONTAL,
        LinearWidgetLayout::PrimaryAxis::SCALE_TO_FIT,
        LinearWidgetLayout::SecondaryAxis::SCALE_TO_FIT
    ));

    check_boxes[0]->setLayoutParams({ .rel_size = {1.f, 1.f} });
    check_boxes[1]->setLayoutParams({ .rel_size = {1.f, 1.f} });
    check_boxes[2]->setLayoutParams({ .rel_size = {1.f, 1.f} });
    check_boxes[3]->setLayoutParams({ .rel_size = {1.f, 1.f} });
    // clang-format on
}

} // namespace bembel::tools