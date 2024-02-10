module;
#include <Windows.h>
#include <assimp/scene.h>

#include <array>
#include <assimp/Importer.hpp>
#include <filesystem>
#include <string_view>
module bembel.tools.editor;

import bembel;
import :ExortDialog;

namespace bembel::tools {
using namespace bembel::base;
using namespace bembel::kernel;
using namespace bembel::graphics;
using namespace bembel::gui;
using namespace bembel::imgui;

static char const* EXORT_DIALOG_TITLE = "Export Assets";

void openExortDialog() {
    imgui::OpenPopup(EXORT_DIALOG_TITLE);
}

std::vector<std::string_view> getListOfDrives() {
    std::vector<std::string_view>     drives;
    static std::array<char, MAX_PATH> buffer;
    ::GetLogicalDriveStringsA(buffer.size(), buffer.data());

    uint begin = 0;
    for(uint i = 0; i < buffer.size(); ++i) {
        if(buffer[i] != '\0') continue;
        drives.emplace_back(&buffer[begin]);
        begin = i + 1;
    }
    return drives;
}

std::filesystem::path drawPathSelection() {
    static std::filesystem::path current_folder = std::filesystem::current_path();

    auto root = current_folder.root_name();

    if(imgui::BeginChild(
           "Path",
           ImVec2(0.0f, 0.0f),
           ImGuiChildFlags_AutoResizeY)) {
        imgui::PushStyleVar(imgui::ImGuiStyleVar_ItemSpacing, imgui::ImVec2(0.0f, 0.0f)
        );
        if(imgui::BeginCombo(
               "##RootDirSelect",
               (char const*)root.u8string().c_str(),
               ImGuiComboFlags_WidthFitPreview
           )) {
            for(std::filesystem::path drive : getListOfDrives()) {
                std::error_code ec;
                if(!std::filesystem::exists(drive, ec) || ec) continue;
                if(imgui::Selectable((char const*)drive.u8string().c_str(), drive == root)) { //
                    current_folder = root = drive;
                }
            }
            imgui::EndCombo();
        }
        std::vector<std::filesystem::path> parts;
        for(auto p = current_folder; p != p.parent_path(); p = p.parent_path()) parts.push_back(p);

        for(auto it = parts.rbegin(); it != parts.rend(); ++it) {
            imgui::SameLine();
            imgui::Text("/");
            imgui::SameLine();
            if(imgui::Button((char const*)it->stem().u8string().c_str())) { current_folder = *it; }
        }
        imgui::PopStyleVar(1);
    }
    imgui::EndChild();

    std::filesystem::path folder = current_folder;
    if(imgui::BeginChild("SubFolder", ImVec2(-FLT_MIN, 100), ImGuiChildFlags_Border)) {
        if(folder != folder.parent_path()) {
            if(imgui::Selectable("..", false)) { //
                current_folder = folder.parent_path();
            }
        }
        for(auto const& it : std::filesystem::directory_iterator{folder}) {
            if(!it.is_directory()) continue;
            auto stem = it.path().stem();
            if(imgui::Selectable((char const*)stem.u8string().c_str(), false)) { //
                current_folder = it.path();
            }
        }
    }
    imgui::EndChild();

    static std::array<char, 256> buffer = {'a', 's', 's', 'e', 't', 's', 0};
    imgui::InputText(".xml", buffer.data(), buffer.size());

    return current_folder / (std::string(buffer.data()) + ".xml");
}

struct ExportSettings {};

ExportSettings const& drawSettingsUI() {
    static ExportSettings settings;
    return settings;
}

void drawExortDialog() {
    ImVec2 center = imgui::GetMainViewport()->GetCenter();
    imgui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    if(!imgui::BeginPopupModal(EXORT_DIALOG_TITLE, NULL, ImGuiWindowFlags_AlwaysAutoResize)) return;

    auto path = drawPathSelection();

    imgui::SeparatorText("Settings");
    auto settings = drawSettingsUI();

    if(imgui::Button("Export", ImVec2(200, 0))) { imgui::CloseCurrentPopup(); }
    imgui::SetItemDefaultFocus();
    imgui::SameLine();
    if(imgui::Button("Cancel", ImVec2(200, 0))) { imgui::CloseCurrentPopup(); }
    imgui::EndPopup();
}

} // namespace bembel::tools