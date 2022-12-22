module;
#include <filesystem>
#include <memory>
#include <string_view>
module bembel.tools.font_converter;

namespace bembel::tools {
using namespace bembel::base;
using namespace bembel::gui;
using namespace bembel::kernel;

Application::Application() : kernel::Application() {
    m_gui_system = m_engine.addSystem<GuiSystem>();

    events::addHandler<WindowShouldCloseEvent>(this);
    events::addHandler<WindowResizeEvent>(this);
    events::addHandler<FileDropEvent>(this);

    auto escapeKey = m_engine.input.keyboard.getKey(Keyboard::ESCAPE);
    escapeKey->press_signal.bind<Application>(this, &Application::quit);
}

Application::~Application() {
    events::removeHandler<WindowShouldCloseEvent>(this);
    events::removeHandler<WindowResizeEvent>(this);
    events::removeHandler<FileDropEvent>(this);
}

bool Application::init() {
    auto& asset_locator = m_engine.assets.getAssetLocator();
    asset_locator.addGenericAssetDirectory("../assets");
    asset_locator.addGenericAssetDirectory("../assets/gui");
    asset_locator.addGenericAssetDirectory("../assets/shader");

    auto display_mode = std::make_shared<WindowDisplayMode>();
    display_mode->setWidth(1200);
    display_mode->setHeight(900);
    display_mode->setAspectRatio(4, 3);

    m_main_window                   = m_engine.display.createWindow();
    m_main_window->background_color = vec3{0.05f, 0.05f, 0.10f};
    m_main_window->setDisplayMode(display_mode);

    m_main_window->createViewport(vec2{0.00f, 0.f}, vec2{0.75f, 1.f});
    m_main_window->createViewport(vec2{0.75f, 0.f}, vec2{0.25f, 1.f});

    m_main_window->open("Font Converter");

    m_converter = std::make_unique<FontConverter>(m_engine.assets);
    m_converter->font_family_added_signal.bind(this, &Application::onFontFamilyAdded);

    if(!initUserInterface()) return false;

    m_main_window->getViewport(1)->addView(&m_gui->view);

    m_font_view = std::make_unique<FontView>(
        m_engine.assets, m_converter->getGlyphTextureAtlas(), m_converter->getTextureGenerator()
    );
    m_main_window->getViewport(0)->addView(m_font_view.get());

    m_widgets.texture_size_slider->setValue(1024);

    m_engine.initSystems();

    return true;
}

void Application::cleanup() {
    m_font_view.reset();
    m_converter.reset();

    m_engine.shutdownSystems();
    m_engine.display.closeOpenWindows();
}

void Application::update(double time) {}

void Application::handleEvent(In<WindowShouldCloseEvent> event) {
    quit();
}

void Application::handleEvent(In<WindowResizeEvent> event) {}

void Application::handleEvent(In<FileDropEvent> event) {
    for(auto& file_path : event.files) { m_converter->loade(file_path); }

    auto font_family = m_converter->getFontFamily();

    if(font_family) {
        for(int i = 0; i < 4; ++i) {
            if(font_family->hasFace(FontFamily::FaceType(i))) {
                m_widgets.type_face_selection[i]->enable();
                m_widgets.type_face_selection[i]->state = CheckBoxWidget::State::SELECTED;
            } else {
                m_widgets.type_face_selection[i]->disable();
            }
        }
    }
}

bool Application::initUserInterface() {
    Asset<ShaderProgram> gui_shader;
    Asset<gui::Style>    gui_style;

    gui_shader.request(m_engine.assets, "gui.shader-program");
    gui_style.request(m_engine.assets, "dark_gui_style.xml");

    if(!gui_shader || !gui_style) return false;

    m_gui = m_gui_system->createGUI("main");
    m_gui->renderer.init(gui_shader, gui_style);

    using LinearWidgetLayout::Mode::SCALE_TO_FIT;

    m_layout = m_gui->root_widget.setLayout<LinearWidgetLayout>(SCALE_TO_FIT);

    m_widgets.createWidgets(m_gui);
    m_widgets.initLayout(m_layout);

    // clang-format off
    m_widgets.load_file_path_input  ->     text.change_signal.bind(this, &Application::onFontFilePathChanged);
    m_widgets.load_file_button      ->           click_signal.bind(this, &Application::lodeFontFile);
    m_widgets.font_family_selections->selection_change_signal.bind(this, &Application::onSelectFontFamily);
    m_widgets.texture_size_slider   ->    value_update_signal.bind(this, &Application::onTextureResulutionUpdate);
    m_widgets.convert_font_button   ->           click_signal.bind(this, &Application::onConvertFont);
    m_widgets.save_font_button      ->           click_signal.bind(this, &Application::onSaveFont);
    // clang-format on

    return true;
}

void Application::lodeFontFile() {
    std::filesystem::path path = m_widgets.load_file_path_input->text.get().data;
    if(!std::filesystem::exists(path)) {
        m_widgets.load_file_error->setText(u8"File dosn't exist");
        return;
    }
    if(!m_converter->loade(path)) {
        m_widgets.load_file_error->setText(u8"Failed to load file.");
        return;
    }
}

void Application::onFontFilePathChanged(In<String>, In<String>) {
    m_widgets.load_file_error->setText(u8"");
}

void Application::onFontFamilyAdded(In<std::u8string_view> name) {
    m_widgets.font_family_selections->addRadioButton(name);
    if(m_widgets.font_family_selections->getSelection() == -1)
        m_widgets.font_family_selections->setSelection(0);

    m_widgets.convert_font_button->enable();

    m_gui->root_widget.updateLayout();
}

void Application::onConvertFont() {
    std::vector<char32_t> chars;
    if(m_widgets.char_set_selection[0]->isSelected()) // Basic Latin
        for(char32_t c = 0x0000; c <= 0x007f; ++c) chars.push_back(c);
    if(m_widgets.char_set_selection[1]->isSelected()) // Latin-1 Supplement
        for(char32_t c = 0x0080; c <= 0x00ff; ++c) chars.push_back(c);
    if(m_widgets.char_set_selection[2]->isSelected()) // Latin Extended-A
        for(char32_t c = 0x0100; c <= 0x017f; ++c) chars.push_back(c);
    if(m_widgets.char_set_selection[3]->isSelected()) // Latin Extended-B
        for(char32_t c = 0x0180; c <= 0x024f; ++c) chars.push_back(c);

    std::vector<FontFamily::FaceType> faces;
    if(m_widgets.type_face_selection[0]->isSelected()) // DEFAULT
        faces.push_back(FontFamily::FaceType::DEFAULT);
    if(m_widgets.type_face_selection[1]->isSelected()) // OBLIQUE
        faces.push_back(FontFamily::FaceType::OBLIQUE);
    if(m_widgets.type_face_selection[2]->isSelected()) // BOLD
        faces.push_back(FontFamily::FaceType::BOLD);
    if(m_widgets.type_face_selection[3]->isSelected()) // BOLD & OBLIQUE
        faces.push_back(FontFamily::FaceType::BOLD_OBLIQUE);

    m_converter->converSelectedFont(
        std::move(chars), faces, m_widgets.sdf_max_dist_slider->getValue()
    );

    m_widgets.save_font_button->enable();
}

void Application::onSaveFont() {
    FontFamily* font = m_converter->getFontFamily();
    if(!font) return;

    std::filesystem::path path = u8"fonts/";
    path.append(font->getName());
    if(!m_converter->save(path)) {}
}

void Application::onSelectFontFamily(int index) {
    m_converter->setSelectedFontFamily(index);

    auto font_family = m_converter->getFontFamily();
    if(!font_family) { return; }
    for(int i = 0; i < 4; ++i) {
        if(font_family->hasFace(FontFamily::FaceType(i))) {
            m_widgets.type_face_selection[i]->enable();
            m_widgets.type_face_selection[i]->state = CheckBoxWidget::State::SELECTED;
        } else {
            m_widgets.type_face_selection[i]->disable();
        }
    }

    m_gui->root_widget.updateLayout();

    m_font_view->setFont(font_family);
}

void Application::onTextureResulutionUpdate(i64 res) {
    m_converter->setResolution(uvec2{res, res});
}

void Application::Widgets::createWidgets(GraphicalUserInterface* gui) {
    auto& root = gui->root_widget;

    load_file_label = root.createChildWidget<LabelWidget>(u8"Load Font File:");
    load_file_label->setHasOutline(true);

    load_file_path_input = root.createChildWidget<TextInputWidget>();
    load_file_button     = root.createChildWidget<ButtonWidget>(u8"Load");
    load_file_error      = root.createChildWidget<LabelWidget>();

    load_file_path_input->text = String(u8"fonts/FreeSans.ttf");
    load_file_error->setTextColor(ColorRGBA{255, 0, 0, 255});

    font_selections_label = root.createChildWidget<LabelWidget>(u8"Font:");
    font_selections_label->setHasOutline(true);

    font_family_selections = root.createChildWidget<RadioButtonGroupWidget>();
    type_face_selection[0] = root.createChildWidget<CheckBoxWidget>(u8"Default");
    type_face_selection[1] = root.createChildWidget<CheckBoxWidget>(u8"Oblique");
    type_face_selection[2] = root.createChildWidget<CheckBoxWidget>(u8"Bold");
    type_face_selection[3] = root.createChildWidget<CheckBoxWidget>(u8"Bold & Oblique");

    type_face_selection[0]->disable();
    type_face_selection[1]->disable();
    type_face_selection[2]->disable();
    type_face_selection[3]->disable();

    char_set_label = root.createChildWidget<LabelWidget>(u8"Character Sets:");
    char_set_label->setHasOutline(true);

    char_set_selection[0] = root.createChildWidget<CheckBoxWidget>(u8"Basic Latin");
    char_set_selection[1] = root.createChildWidget<CheckBoxWidget>(u8"Latin-1 Supplement");
    char_set_selection[2] = root.createChildWidget<CheckBoxWidget>(u8"Latin Extended-A");
    char_set_selection[3] = root.createChildWidget<CheckBoxWidget>(u8"Latin Extended-B");

    char_set_selection[0]->state = CheckBoxWidget::State::SELECTED;

    additional_chars_label = root.createChildWidget<LabelWidget>(u8"Other Chars:");
    additional_chars_input = root.createChildWidget<TextInputWidget>();

    sdf_label = root.createChildWidget<LabelWidget>(u8"SDF Texture:");
    sdf_label->setHasOutline(true);

    texture_size_label  = root.createChildWidget<LabelWidget>(u8"Resolution:");
    texture_size_slider = root.createChildWidget<IntSliderWidget>(256, 4 * 1024, true);
    sdf_max_dist_label  = root.createChildWidget<LabelWidget>(u8"Max distance:");
    sdf_max_dist_slider = root.createChildWidget<IntSliderWidget>(2, 16);

    convert_font_button = root.createChildWidget<ButtonWidget>(u8"Convert Font");
    convert_font_button->disable();

    save_font_button = root.createChildWidget<ButtonWidget>(u8"Save Font");
    save_font_button->disable();

    save_file_error = root.createChildWidget<LabelWidget>();
    save_file_error->setTextColor(ColorRGBA{255, 0, 0, 255});
}

void Application::Widgets::initLayout(LinearWidgetLayout* layout) {
    layout->setMargin(8, 8);

    // clang-format off
    layout->addRow({.min_height = 8, .rel_height = 0.0});
    layout->addRow({.height = 32}).addWidget(load_file_label);
    layout->addRow({.height = 32}).addWidget(load_file_path_input).addSpacing(2).addWidget(load_file_button, 0.f, 64);
    layout->addRow({.height = 28}).addWidget(load_file_error);
    layout->addRow({.rel_height = 1.0});
    layout->addRow({.height = 32}).addWidget(font_selections_label);
    layout->addRow().addWidget(font_family_selections);
    layout->addRow({.height = 28}).addWidget(type_face_selection[0]).addWidget(type_face_selection[1]);
    layout->addRow({.height = 28}).addWidget(type_face_selection[2]).addWidget(type_face_selection[3]);
    layout->addRow({.rel_height = 1.0});
    layout->addRow({.height = 32}).addWidget(char_set_label);
    layout->addRow({.height = 28}).addWidget(char_set_selection[0]);
    layout->addRow({.height = 28}).addWidget(char_set_selection[1]);
    layout->addRow({.height = 28}).addWidget(char_set_selection[2]);
    layout->addRow({.height = 28}).addWidget(char_set_selection[3]);
    layout->addRow({.height = 28}).addWidget(additional_chars_label, 0.f, 128).addWidget(additional_chars_input);
    layout->addRow({.rel_height = 1.0});
    layout->addRow({.height = 32}).addWidget(sdf_label);
    layout->addRow({.height = 2});
    layout->addRow({.height = 28}).addWidget(texture_size_label, 0.f, 128).addWidget(texture_size_slider);
    layout->addRow({.height = 2});
    layout->addRow({.height = 28}).addWidget(sdf_max_dist_label, 0.f, 128).addWidget(sdf_max_dist_slider);
    layout->addRow({.rel_height = 1.0});
    layout->addRow({.height = 32}).addWidget(convert_font_button);
    layout->addRow({.height = 2});
    layout->addRow({.height = 32}).addWidget(save_font_button);
    layout->addRow({.height = 28}).addSpacing(4).addWidget(save_file_error);
    layout->addRow({.rel_height = 1.0});
    // clang-format off
}

} // namespace bembel::tools
