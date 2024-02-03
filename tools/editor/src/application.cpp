module;
#include <assimp/postprocess.h> // Post processing flags
#include <assimp/scene.h>       // Output data structure

#include <assimp/Importer.hpp> // C++ importer interface
#include <filesystem>
#include <memory>
#include <numbers>
#include <string_view>

module bembel.tools.editor;

import bembel;
import :Textures;
import :Materials;
import :Meshes;
import :Models;
import :Scenes;

namespace bembel::tools {
using namespace bembel::base;
using namespace bembel::kernel;
using namespace bembel::graphics;
using namespace bembel::gui;
using namespace bembel::physics;

Application::Application() {}

Application::~Application() {}

bool Application::init(std::span<std::string_view> args) {
    if(!m_engine.loadSetting("editor/config.xml")) return false;

    m_main_window = m_engine.display.getWindow(0);
    if(!m_main_window) return false;

    auto* viewport = m_main_window->getViewport(0);
    if(!viewport) return false;

    m_pipeline = getSystem<GraphicSystem>()->getRenderingPipelines()[0].get();
    if(!m_pipeline) return false;

    viewport->addView(this);

    initDefaultScenes(m_pipeline);

    m_engine.initSystems();

    return true;
}

void Application::cleanup() {
    TEXTURES.clear();
    MATERIALS.clear();
    MESHES.clear();
    MODELS.clear();
    MODEL_PREVIEW_ENTITY = Entity{};
    SCENES.clear();
    m_engine.shutdownSystems();
    m_engine.display.closeOpenWindows();
}

void Application::update(double time) {}

void Application::handleEvent(In<WindowShouldCloseEvent> event) {
    quit();
}

void Application::drawUI(In<ivec2> view_port_pos, In<uvec2> view_port_size) {
    auto viewport = imgui::GetMainViewport();

    imgui::SetNextWindowPos(imgui::ImVec2(view_port_pos.x, view_port_pos.y));
    imgui::SetNextWindowSize(imgui::ImVec2(view_port_size.x, view_port_size.y));
    imgui::SetNextWindowViewport(viewport->ID);
    imgui::PushStyleVar(imgui::ImGuiStyleVar_WindowRounding, 0.0f);
    imgui::PushStyleVar(imgui::ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(imgui::ImGuiStyleVar_WindowPadding, imgui::ImVec2(0.0f, 0.0f));

    int dockspace_flags = imgui::ImGuiDockNodeFlags_None;
    int window_flags    = imgui::ImGuiWindowFlags_MenuBar | imgui::ImGuiWindowFlags_NoDocking
                     | imgui::ImGuiWindowFlags_NoTitleBar | imgui::ImGuiWindowFlags_NoCollapse
                     | imgui::ImGuiWindowFlags_NoResize | imgui::ImGuiWindowFlags_NoMove
                     | imgui::ImGuiWindowFlags_NoBackground
                     | imgui::ImGuiWindowFlags_NoBringToFrontOnFocus
                     | imgui::ImGuiWindowFlags_NoNavFocus;

    if(imgui::Begin("DockSpace", nullptr, window_flags)) {
        if(imgui::BeginMenuBar()) {
            if(imgui::BeginMenu("File")) {
                if(imgui::MenuItem("Import", "Ctrl+I")) { /* Do stuff */
                }
                if(imgui::MenuItem("Save", "Ctrl+S")) { /* Do stuff */
                }
                if(imgui::MenuItem("Close", "Ctrl+W")) quit();
                imgui::EndMenu();
            }
            imgui::EndMenuBar();
        }
        auto dockspace_id = imgui::GetID("MyDockSpace");
        imgui::DockSpace(dockspace_id, {0.0f, 0.0f}, dockspace_flags);
    }
    imgui::End();
    imgui::PopStyleVar(3);

    ImGui::PushStyleVar(imgui::ImGuiStyleVar_WindowPadding, imgui::ImVec2(0.0f, 0.0f));
    imgui::Begin("SceneView", nullptr, imgui::ImGuiWindowFlags_None);
    drawScenePreviewUI(m_pipeline);

    imgui::End();
    imgui::PopStyleVar(1);

    imgui::Begin("Textures", nullptr, imgui::ImGuiWindowFlags_None);
    drawTextureManagementUI();
    imgui::End();
    imgui::Begin("Materials", nullptr, imgui::ImGuiWindowFlags_None);
    drawMaterialManagementUI();
    imgui::End();
    imgui::Begin("Meshes", nullptr, imgui::ImGuiWindowFlags_None);
    drawMeshManagementUI();
    imgui::End();
    imgui::Begin("Models", nullptr, imgui::ImGuiWindowFlags_None);
    drawModelManagementUI();
    imgui::End();

    // imgui::ShowDemoWindow();
}

void Application::handleEvent(In<FrameBufferResizeEvent> event) {}

void Application::handleEvent(In<FileDropEvent> event) {
    for(auto file : event.files) {
        std::string file_name = file.string();

        aiScene const* scene = m_importer.ReadFile(
            file_name.c_str(),
            aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_JoinIdenticalVertices
                | aiProcess_SortByPType
        );
        if(!scene) continue;

        uint material_index_offset = MATERIALS.size();
        for(auto mat : std::span(scene->mMaterials, scene->mNumMaterials)) { //
            parseMaterial(mat);
        }
        for(auto mesh : std::span(scene->mMeshes, scene->mNumMeshes)) {
            parseModels(mesh, material_index_offset);
        }
    }
}

} // namespace bembel::tools
