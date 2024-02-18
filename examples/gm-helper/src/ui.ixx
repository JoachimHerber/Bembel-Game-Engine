module;
#include <array>
#include <string>
export module bembel.examples.gm_helper:UI;

import bembel;
import :Components;
import :Selection;
import :Texture;

namespace bembel::examples::gm_helper {
using namespace base;
using namespace kernel;
using namespace graphics;
using namespace gui;

bool g_open_context_menu = false;
vec2 g_cursor_pos        = {0.f, 0.f};

constexpr auto TOKEN_SIZE_NAMES =
    std::array{"Tiny", "Small", "Medium", "Large", "Huge", "Gargantuan"};
constexpr auto TOKEN_TYPE_NAMES = std::array{"Player", "Monster", "NPC"};

export void openContextMenu(In<vec2> cursor_pos) {
    g_open_context_menu = true;
    g_cursor_pos        = cursor_pos;
}

enum class Unit { FT, METER, FIELDS };
constexpr auto UNIT_NAMES = std::array{"ft.", "Meter", "Fields"};

void drawLightRangeUI(InOut<vec2> range) {
    static Unit g_unit = Unit::FT;

    switch(g_unit) {
        case Unit::FT:
            range *= 5.f;
            imgui::DragFloatRange2(
                "##Range", &range.x, &range.y, 5.f, 0.0f, 120.0f, "Bright: %.0f", "Dim: %.0f"
            );
            range /= 5.f;
            break;
        case Unit::METER:
            range *= 1.5f;
            imgui::DragFloatRange2(
                "##Range", &range.x, &range.y, 1.0f, 0.0f, 90.0f, "Bright: %.1f", "Dim: %.1f"
            );
            range /= 1.5f;
            break;
        case Unit::FIELDS:
            imgui::DragFloatRange2(
                "##Range", &range.x, &range.y, 1.f, 0.0f, 60.0f, "Bright: %.0f", "Dim: %.0f"
            );
            break;
    }
    imgui::SameLine();
    imgui::PushItemWidth(60);
    if(imgui::BeginCombo("Light", UNIT_NAMES[uint(g_unit)], 0)) {
        for(uint n = 0; n < UNIT_NAMES.size(); n++) {
            if(imgui::Selectable(UNIT_NAMES[n], g_unit == Unit(n))) g_unit = Unit(n);
        }
        imgui::EndCombo();
    }
}

void drawEditTokenDialog(In<Camera*> cam, InOut<Entity> selected_entity) {
    auto* token = selected_entity.get<Token>();
    if(!token) return;

    // if(token->type == Token::Type::Other) {
    //     auto* transform = selected_entity.get<Transform>();
    //     auto* sprite    = selected_entity.get<Sprite>();
    //     imgui::DragFloat("Width", &(sprite->size.x), 0.5f, 0.5f, 20.0f, "%.1f");
    //     imgui::SameLine();
    //     imgui::DragFloat("Height", &(sprite->size.y), 0.5f, 0.5f, 20.0f, "%.1f");
    //     selected_entity.get<Selectable>()->dist = 0.5f * std::min(sprite->size.x,
    //     sprite->size.y);
    //
    //     imgui::DragFloat("Rotation", &(sprite->rotation), 5.f, -180.f, 180.0f, "%f°");
    // } else {
    uint selected_size = uint(token->size);
    if(imgui::BeginCombo("Size", TOKEN_SIZE_NAMES[selected_size], 0)) {
        for(uint n = 0; n < TOKEN_SIZE_NAMES.size(); n++) {
            if(imgui::Selectable(TOKEN_SIZE_NAMES[n], selected_size == n)) {
                token->size = Token::Size(n);
                // selected_entity.get<Sprite>()->size     = vec2(TOKEN_SIZE_VALUES[n]);
                selected_entity.get<Selectable>()->dist = 0.5f * TOKEN_SIZE_VALUES[n];
            }
        }
        imgui::EndCombo();
    }
    //}

    bool emit_light = selected_entity.has<Light>();
    imgui::Checkbox("Hidden", &(token->hidden));
    imgui::SameLine();
    if(imgui::Checkbox("Emit Light", &emit_light)) {
        if(emit_light) {
            selected_entity.assign<Light>(vec2(0.f, 0.f));
        } else {
            selected_entity.remove<Light>();
        }
    }
    if(auto* light = selected_entity.get<Light>()) { drawLightRangeUI(light->range); }

    if(imgui::Button("Focus", ImVec2(80, 0))) {
        auto pos = selected_entity.get<Transform>()->position;
        cam->setPosition(vec3(pos.x, pos.y, 0));
        imgui::CloseCurrentPopup();
    }

    imgui::SameLine();

    if(imgui::Button("Delete", ImVec2(80, 0))) {
        updateSelection({});
        selected_entity.deleteEntity();
        imgui::CloseCurrentPopup();
    }
}

std::optional<uint> drawTextrueSelection(In<std::vector<Asset<Texture>>> textures) {
    static constexpr uint image_width = 64;
    auto const&           style       = imgui::GetStyle();

    std::optional<uint> texture;
    if(imgui::BeginChild("Path", ImVec2(640.0f, 450.0f))) {
        for(uint n = 0; n < textures.size(); n++) {
            if(imgui::ImageButton(
                   (void*)(intptr_t)textures[n]->getTextureHandle(),
                   imgui::ImVec2(image_width, image_width),
                   imgui::ImVec2(0.0f, 1.0f),
                   imgui::ImVec2(1.0f, 0.0f)
               )) {
                texture = n;
            }

            float last_x = imgui::GetItemRectMax().x;
            float next_x = last_x + style.ItemSpacing.x + image_width;
            if(n + 1 < textures.size() && n % 8 != 7) imgui::SameLine();
        }
    }
    imgui::EndChild();
    return texture;
}

Token::Type drawTokenTypeComboBox() {
    static uint type = Token::Type::Player;
    if(imgui::BeginCombo("Type", TOKEN_TYPE_NAMES[type], 0)) {
        for(int n = 0; n < TOKEN_TYPE_NAMES.size(); n++) {
            if(imgui::Selectable(TOKEN_TYPE_NAMES[n], type == n)) { type = n; }
        }
        imgui::EndCombo();
    }
    return Token::Type(type);
}

Token::Size drawTokenSizeComboBox() {
    static uint size = Token::Size::Medium;
    if(imgui::BeginCombo("Size", TOKEN_SIZE_NAMES[size], 0)) {
        for(int n = 0; n < TOKEN_SIZE_NAMES.size(); n++) {
            if(imgui::Selectable(TOKEN_SIZE_NAMES[n], size == n)) { size = n; }
        }
        imgui::EndCombo();
    }
    return Token::Size(size);
}

void drawCreateTokenDialog(In<Scene*> scene) {
    if(imgui::BeginTabBar("##TabBar")) {
        if(imgui::BeginTabItem("Token")) {
            auto type = drawTokenTypeComboBox();
            imgui::SameLine();
            auto size = drawTokenSizeComboBox();
            if(auto texture_idx = drawTextrueSelection(g_token_textures)) {
                imgui::CloseCurrentPopup();

                auto& texture = g_token_textures[*texture_idx];
                auto& name    = g_token_names[*texture_idx];

                Entity enity{scene};
                enity.assign<Transform>(vec3(g_cursor_pos, 0.f));
                enity.assign<Token>(type, size, name, texture, type == Token::Type::Monster);
                enity.assign<Selectable>(0.5f * TOKEN_SIZE_VALUES[size]);
                if(type == Token::Type::Player) enity.assign<Light>(vec2(0.f, 0.f));
            }
            imgui::EndTabItem();
        }
        if(imgui::BeginTabItem("Other")) {
            // static bool emit_light = true;
            // imgui::Checkbox("Emit Light", &emit_light);
            // if(auto texture = drawTextrueSelection(g_token_textures_other)) {
            //     imgui::CloseCurrentPopup();
            //
            //     Entity enity{scene};
            //     enity.assign<Transform>(vec3(g_cursor_pos, 0.f));
            //     enity.assign<Sprite>(texture, vec2(1.f));
            //     enity.assign<Token>(Token::Size::Medium, Token::Type::Other, true);
            //     enity.assign<Selectable>(0.5f);
            //     if(emit_light) enity.assign<Light>(vec2(4.f, 8.f));
            // }
            imgui::EndTabItem();
        }
        imgui::EndTabBar();
    }

    // imgui::SameLine();
    // imgui::Checkbox("Hidden", &hidden);
    // imgui::SameLine();
    // imgui::Checkbox("Emit Light", &emit_light);
}

export void drawUI(In<Scene*> scene, In<Camera*> cam) {
    constexpr char const* CONTEXT_MENU = "##CONTEXT_MENU";

    if(g_open_context_menu) {
        imgui::OpenPopup(CONTEXT_MENU);
        g_open_context_menu = false;
    }

    auto& style = imgui::GetStyle();

    imgui::ImVec2 center = imgui::GetMainViewport()->GetCenter();
    imgui::SetNextWindowPos(center, imgui::ImGuiCond_Appearing, imgui::ImVec2(0.5f, 0.5f));
    bool opened = true;
    if(!imgui::BeginPopupModal(CONTEXT_MENU, &opened, imgui::ImGuiWindowFlags_AlwaysAutoResize))
        return;

    if(auto selected_entity = getSelection()) {
        drawEditTokenDialog(cam, *selected_entity);
    } else {
        drawCreateTokenDialog(scene);
    }

    imgui::EndPopup();
}

} // namespace bembel::examples::gm_helper