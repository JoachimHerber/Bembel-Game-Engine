module;
#include <array>
#include <format>
#include <string>
module bembel.tools.editor;

import bembel;
import :Textures;

namespace bembel::tools {

void drawTextureManagementUI() {
    static std::array<char, 256> buffer;
    static std::string           error;
    static constexpr uint        image_width = 200;

    float window_visible_x2 = imgui::GetWindowPos().x + imgui::GetWindowContentRegionMax().x;

    auto& style = imgui::GetStyle();
    for(int n = 0; n < TEXTURES.size(); n++) {
        imgui::BeginGroup();
        imgui::Text(TEXTURES[n].name.c_str());
        imgui::Image(
            (void*)(intptr_t)TEXTURES[n].asset->getTextureHandle(),
            imgui::ImVec2(image_width, image_width),
            imgui::ImVec2(0.0f, 1.0f),
            imgui::ImVec2(1.0f, 0.0f)
        );
        imgui::EndGroup();

        float last_button_x2 = imgui::GetItemRectMax().x;
        float next_button_x2 = last_button_x2 + style.ItemSpacing.x
                             + image_width; // Expected position if next button was on same line
        if(n + 1 < TEXTURES.size() && next_button_x2 < window_visible_x2) imgui::SameLine();
    }

    if(imgui::InputText("##TexturePathInput", buffer.data(), buffer.size())) { error = ""; }
    imgui::SameLine();
    if(imgui::Button("Load##LoadTextureButton")) {
        std::string_view path = buffer.data();
        Asset<Texture>   texture;
        texture.request(path);
        if(texture) {
            TEXTURES.emplace_back(std::string(path), texture);
        } else {
            error = std::format("Failed to load '{}'", path);
        }
    }
    imgui::Text(error.c_str());
}

bool drawTextureSelectionUI(In<char const*> label, InOut<std::optional<uint>> index) {
    char const* combo_preview_value = "---";
    if(index && *index < TEXTURES.size()) combo_preview_value = TEXTURES[*index].name.c_str();

    bool selection_chaged = false;
    if(imgui::BeginCombo(label, combo_preview_value, 0)) {
        if(imgui::Selectable("---", !index)) {
            selection_chaged = true;
            index            = {};
        }
        for(int n = 0; n < TEXTURES.size(); n++) {
            if(imgui::Selectable(TEXTURES[n].name.c_str(), index && *index == n)) {
                selection_chaged = true;
                index            = n;
            }
        }
        imgui::EndCombo();
    }

    return selection_chaged;
}

} // namespace bembel::tools