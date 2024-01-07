module;
#include <assimp/postprocess.h> // Post processing flags
#include <assimp/scene.h>       // Output data structure

#include <assimp/Importer.hpp> // C++ importer interface
#include <filesystem>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <memory>
#include <string_view>
module bembel.tools.editor;

namespace bembel::tools {
using namespace bembel::base;
using namespace bembel::kernel;
using namespace bembel::graphics;
using namespace bembel::gui;
using namespace bembel::physics;

Application::Application() {}

Application::~Application() {}

bool Application::init(std::span<std::string_view> args) {
    auto& asset_locator = AssetLocator::getInstance();
    asset_locator.addGenericAssetDirectory("../assets");
    asset_locator.addGenericAssetDirectory("../assets/gui");
    asset_locator.addGenericAssetDirectory("../assets/shader");
    asset_locator.addGenericAssetDirectory("../assets/fonts");

    m_main_window                   = m_engine.display.createWindow();
    m_main_window->background_color = vec3{0.9f, 0.9f, 0.9f};
    m_main_window->setDisplayMode(std::make_shared<WindowDisplayMode>(uvec2{1200, 900}));

    auto& gui_viewport     = m_main_window->createViewport(vec2{0.00f, 0.f}, vec2{1.f, 1.f});
    auto& preview_viewport = m_main_window->createViewport();

    m_main_window->open("Bembel Editor");

    initRenderingPipeline(preview_viewport);
    initUserInterface(preview_viewport);
    gui_viewport.addView(&m_gui->view);

    m_material_mgr = std::make_unique<MaterialManager>(m_tabs->addTab(u8"Materials"), m_pipeline);
    m_model_mgr    = std::make_unique<ModelManager>(
        m_tabs->addTab(u8"Models"), m_pipeline, m_material_mgr.get()
    );

    m_engine.initSystems();

    return true;
}

void Application::cleanup() {
    m_material_mgr.reset();
    m_engine.shutdownSystems();
    m_engine.display.closeOpenWindows();
}

void Application::update(double time) {}

void Application::handleEvent(In<WindowShouldCloseEvent> event) {
    quit();
}

void Application::handleEvent(In<FrameBufferResizeEvent> event) {
    m_tabs->size = event.size;
}

void Application::handleEvent(In<FileDropEvent> event) {
    for(auto file : event.files) {
        auto        importer  = std::make_unique<Assimp::Importer>();
        std::string file_name = file.string();

        aiScene const* scene = importer->ReadFile(
            file_name.c_str(),
            aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_JoinIdenticalVertices
                | aiProcess_SortByPType
        );
        if(!scene) continue;

        m_imports.emplace_back(std::move(importer), scene);
        m_material_mgr->parseScene(scene);
        m_model_mgr->parseScene(scene);
    }
}

bool Application::initUserInterface(Viewport& viewport) {
    Asset<ShaderProgram> gui_shader;
    Asset<gui::Style>    gui_style;

    gui_shader.request("gui.shader-program");
    gui_style.request("dark_gui_style.xml");

    if(!gui_shader || !gui_style) return false;

    m_gui = getSystem<GuiSystem>()->createGUI("main");
    m_gui->renderer.init(gui_shader, gui_style);

    m_root = m_gui->root_widget.createChildWidget<SplitGroupWidget>();

    m_root->size     = m_main_window->getFrameBufferSize();
    m_root->position = ivec2{0, 0};
    m_root->setSeperatorPos(m_main_window->getFrameBufferSize().y);

    auto preview = m_root->getFirstChildGroup().createChildWidget<ViewportWidget>(
        &viewport, m_pipeline->getCamera().get()
    );

    m_tabs = m_root->getSecondChildGroup().createChildWidget<TabGroupWidget>();

    m_gui->root_widget.setLayout<RelativeWidgetLayout>()->addWidget(
        m_root, {0.0f, 0.0f}, {1.0f, 1.0f}
    );
    m_root->getFirstChildGroup().setLayout<RelativeWidgetLayout>()->addWidget(
        preview, {0.0f, 0.0f}, {1.0f, 1.0f}
    );
    m_root->getSecondChildGroup().setLayout<RelativeWidgetLayout>()->addWidget(
        m_tabs, {0.0f, 0.0f}, {1.0f, 1.0f}
    );
    m_root->getFirstChildGroup().updateLayout();
    m_root->getSecondChildGroup().updateLayout();

    return true;
}

bool Application::initRenderingPipeline(Viewport& viewport) {
    auto renderer = getSystem<GraphicSystem>()->setRenderer<DefaultGeometryRenderer>(
        GeometryMesh::VertexFormat::Default
    );

    Asset<ShaderProgram> geomety_pass_shader;
    Asset<ShaderProgram> depth_pass_shader;
    Asset<ShaderProgram> dir_light_shader;
    Asset<ShaderProgram> point_light_shader;
    Asset<ShaderProgram> environment_map_shader;

    geomety_pass_shader.request("normal-mapped-geomety-pass.xml");
    depth_pass_shader.request("default-shadow-pass.xml");
    dir_light_shader.request("dir-light.xml");
    point_light_shader.request("point-light.xml");
    environment_map_shader.request("environment-map-reflections.xml");

    if(!geomety_pass_shader || !depth_pass_shader || !dir_light_shader || !point_light_shader
       || !environment_map_shader)
        return false;
    renderer->setShaders(geomety_pass_shader, depth_pass_shader);

    m_pipeline = getSystem<GraphicSystem>()->createRenderingPipline();

    m_pipeline->init();
    m_pipeline->setResulution({900, 900});

    m_pipeline->createTexture("depth", Texture::Format::DEPTH_COMPONENT24);
    m_pipeline->createTexture("light", Texture::Format::RGB16F);
    m_pipeline->createTexture("albedo", Texture::Format::RGB8);
    m_pipeline->createTexture("material", Texture::Format::RGB8);
    m_pipeline->createTexture("normal", Texture::Format::RGB16);

    auto* geom_stage = m_pipeline->addRenderingStage<GeometryRenderingStage>();
    geom_stage->setDepthOutputTexture("depth");
    geom_stage->setColorOutputTexture(0, "light");
    geom_stage->setColorOutputTexture(1, "albedo");
    geom_stage->setColorOutputTexture(2, "material");
    geom_stage->setColorOutputTexture(3, "normal");

    auto* light_stage = m_pipeline->addRenderingStage<DeferredLightingStage>();
    light_stage->setDirLightShader(dir_light_shader);
    light_stage->setPointLightShader(point_light_shader);
    light_stage->setDirLightShadowResolution(2048);
    light_stage->setDirLightShadowCascadeDistances(15, 50, 100);
    light_stage->setColorOutputTexture(0, "light");
    light_stage->setInputTextures("depth", "albedo", "material", "normal");

    auto* reflect_stage = m_pipeline->addRenderingStage<EnvironmentMapReflectionStage>();
    reflect_stage->setShader(environment_map_shader);
    reflect_stage->setColorOutputTexture(0, "light");
    reflect_stage->setInputTextures("depth", "albedo", "material", "normal");
    reflect_stage->initEnvironmentMap(
        "skybox/X-.png",
        "skybox/X+.png",
        "skybox/Y-.png",
        "skybox/Y+.png",
        "skybox/Z+.png",
        "skybox/Z-.png"
    );
    auto* view = m_pipeline->createView("light");
    view->updateViewArea(m_pipeline->getResulution());
    viewport.addView(view);

    auto* cam   = m_pipeline->getCamera().get();
    quat  pitch = glm::angleAxis(0.f, vec3(1, 0, 0));
    quat  yaw   = glm::angleAxis(0.f, vec3(0, 1, 0));
    cam->setOrientation(yaw * pitch);
    cam->setPosition(vec3(0, 0, 3));
    cam->setUpProjection(60.0f * 3.14159265359f / 180.0f, 1, 0.1f, 1000.0f);
    return true;
}

} // namespace bembel::tools
