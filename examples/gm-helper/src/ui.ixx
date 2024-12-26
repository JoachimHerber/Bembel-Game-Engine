export module bembel.examples.gm_helper:UI;

import std;
import bembel;
import :Components;
import :Selection;
import :Texture;
import :Maps;
import :Initiative;

namespace bembel::examples::gm_helper {
using namespace base;
using namespace kernel;
using namespace graphics;
using namespace gui;

bool g_show_initative_traker = false;
bool g_open_context_menu     = false;
vec2 g_cursor_pos            = {0.f, 0.f};

constexpr auto TOKEN_SIZE_NAMES =
    std::array{"Tiny", "Small", "Medium", "Large", "Huge", "Gargantuan"};
constexpr auto TOKEN_TYPE_NAMES = std::array{"Player", "Monster", "NPC"};
constexpr auto AREA_SHAPE_NAMES = std::array{"Sphere", "Box", "Cone", "Line"};

export void openContextMenu(In<vec2> cursor_pos) {
    g_open_context_menu = true;
    g_cursor_pos        = cursor_pos;
}

enum class Unit { FT, METER, FIELDS };
constexpr auto UNIT_NAMES = std::array{"ft.", "Meter", "Fields"};
constexpr auto UNIT_SCALE = std::array{5.f, 1.5f, 1.f};

namespace imgui {
    using namespace bembel::gui::imgui;

    static Unit g_unit = Unit::FT;
    void        UnitComboBox(char const* lable) {
        if(imgui::BeginCombo(lable, UNIT_NAMES[uint(g_unit)], 0)) {
            for(uint n = 0; n < UNIT_NAMES.size(); n++) {
                if(imgui::Selectable(UNIT_NAMES[n], g_unit == Unit(n))) g_unit = Unit(n);
            }
            imgui::EndCombo();
        }
    }

    void DragDistance(const char* lable, float* value, float min, float max) {
        float scale = UNIT_SCALE[uint(g_unit)];
        *value *= scale;
        imgui::DragFloat(
            lable, value, scale, scale * min, scale * max, g_unit == Unit::METER ? "%.1f" : "%.0f"
        );
        *value /= scale;
    }

    void DragLightRange(InOut<vec2> range) {
        imgui::SetNextItemWidth(65);
        imgui::DragDistance("##bright", &range.x, 0.f, 12.f);
        imgui::SameLine();
        imgui::SetNextItemWidth(65);
        imgui::DragDistance("##dim", &range.y, 0.f, 24.f);
        imgui::SameLine();
        imgui::SetNextItemWidth(60);
        imgui::UnitComboBox("Light");
    }

    void EditTokenDialog(InOut<Entity> selected_entity) {
        auto* token = selected_entity.get<Token>();
        if(!token) return;

        uint selected_size = uint(token->size);
        if(imgui::BeginCombo("Size", TOKEN_SIZE_NAMES[selected_size], 0)) {
            for(uint n = 0; n < TOKEN_SIZE_NAMES.size(); n++) {
                if(imgui::Selectable(TOKEN_SIZE_NAMES[n], selected_size == n)) {
                    token->size                             = Token::Size(n);
                    selected_entity.get<Selectable>()->dist = 0.5f * TOKEN_SIZE_VALUES[n];
                }
            }
            imgui::EndCombo();
        }
        imgui::ColorEdit4("Color", glm::value_ptr(token->color));
        imgui::Checkbox("Hidden", &(token->hidden));
        imgui::SameLine();
        imgui::Checkbox("Is Player", &(token->is_player));

        bool emit_light = selected_entity.has<Light>();
        imgui::SeparatorText("Light");
        if(imgui::Checkbox("Emit Light", &emit_light)) {
            if(emit_light) {
                selected_entity.assign<Light>(vec2(0.f, 0.f));
            } else {
                selected_entity.remove<Light>();
            }
        }
        if(auto* light = selected_entity.get<Light>()) //
            imgui::DragLightRange(light->range);

        imgui::SeparatorText("Initiative");
        {
            auto*      initiative = selected_entity.get<Initiative>();
            static int roll       = 10;
            static int bonus      = 1;
            if(initiative) {
                roll  = initiative->roll;
                bonus = initiative->bonus;
            }
            imgui::SetNextItemWidth(40);
            imgui::InputInt("##Roll", &roll, 0);
            imgui::SameLine();
            if(imgui::Button("Roll")) roll = std::rand() % 20 + 1 + bonus;
            imgui::SameLine();
            imgui::SetNextItemWidth(30);
            imgui::InputInt("Bonus", &bonus, 0);
            if(initiative) {
                initiative->roll  = roll;
                initiative->bonus = bonus;
                if(imgui::Button("Remove from Initiative Traker")) //
                    selected_entity.remove<Initiative>();
            } else {
                if(imgui::Button("Add to Initiative Traker")) {
                    selected_entity.assign<Initiative>(roll, bonus);
                    g_show_initative_traker = true;
                }
            }
        }

        imgui::Separator();

        if(imgui::Button("Close", ImVec2(100, 0))) { imgui::CloseCurrentPopup(); }

        imgui::SameLine();

        if(imgui::Button("Delete", ImVec2(100, 0))) {
            updateSelection(std::optional<Entity>{});
            selected_entity.deleteEntity();
            imgui::CloseCurrentPopup();
        }
    }

    void EditAreaMarkerDialog(InOut<Entity> selected_entity) {
        auto* area_marker = selected_entity.get<AreaMarker>();
        if(!area_marker) return;

        uint selected_shape = uint(area_marker->shape);
        if(imgui::BeginCombo("Shape", AREA_SHAPE_NAMES[selected_shape], 0)) {
            for(uint n = 0; n < AREA_SHAPE_NAMES.size(); n++) {
                if(imgui::Selectable(AREA_SHAPE_NAMES[n], selected_shape == n)) {
                    area_marker->shape = AreaMarker::Shape(n);
                }
            }
            imgui::EndCombo();
        }
        imgui::ColorEdit4("Color", glm::value_ptr(area_marker->color));
        imgui::Checkbox("Hidden", &(area_marker->hidden));
        imgui::SameLine();
        imgui::Checkbox("Block Light", &(area_marker->block_light));

        if(imgui::Button("Close", ImVec2(100, 0))) { imgui::CloseCurrentPopup(); }

        imgui::SameLine();

        if(imgui::Button("Delete", ImVec2(100, 0))) {
            updateSelection(std::optional<Entity>{});
            selected_entity.deleteEntity();
            imgui::CloseCurrentPopup();
        }
    }

    void EditDialog(InOut<Entity> selected_entity) {
        if(selected_entity.has<Token>()) {
            EditTokenDialog(selected_entity);
        } else if(selected_entity.has<AreaMarker>()) {
            EditAreaMarkerDialog(selected_entity);
        } else {
            imgui::CloseCurrentPopup();
        }
    }

    std::optional<uint> TextureSelection(In<std::vector<Asset<Texture>>> textures) {
        static constexpr uint image_width = 64;
        auto const&           style       = imgui::GetStyle();

        std::optional<uint> texture;
        if(imgui::BeginChild("Path", ImVec2(640.0f, 450.0f))) {
            for(uint n = 0; n < textures.size(); n++) {
                if(imgui::ImageButton(
                       (void*)(std::intptr_t)textures[n]->getTextureHandle(),
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

    Token::Size TokenSizeComboBox() {
        static uint size = Token::Size::Medium;
        if(imgui::BeginCombo("Size", TOKEN_SIZE_NAMES[size], 0)) {
            for(int n = 0; n < TOKEN_SIZE_NAMES.size(); n++) {
                if(imgui::Selectable(TOKEN_SIZE_NAMES[n], size == n)) { size = n; }
            }
            imgui::EndCombo();
        }
        return Token::Size(size);
    }

    AreaMarker::Shape AreaShapeComboBox() {
        static uint shape = AreaMarker::Shape::SPHERE;
        if(imgui::BeginCombo("Shape", AREA_SHAPE_NAMES[shape], 0)) {
            for(int n = 0; n < AREA_SHAPE_NAMES.size(); n++) {
                if(imgui::Selectable(AREA_SHAPE_NAMES[n], shape == n)) { shape = n; }
            }
            imgui::EndCombo();
        }
        return AreaMarker::Shape(shape);
    }

    void CreateEntityDialog() {
        static vec4  color{1.f, 0.8f, 0.f, 1.f};
        static bool  hidden      = false;
        static bool  block_light = false;
        static float size        = 1.0f;
        if(imgui::BeginTabBar("##TabBar")) {
            if(imgui::BeginTabItem("Token")) {
                imgui::ColorEdit4("##Color", glm::value_ptr(color));
                imgui::SameLine();
                auto size = imgui::TokenSizeComboBox();
                imgui::SameLine();
                imgui::Checkbox("Hidden", &(hidden));
                if(auto texture_idx = imgui::TextureSelection(g_token_textures)) {
                    imgui::CloseCurrentPopup();

                    auto& texture = g_token_textures[*texture_idx];
                    auto& name    = g_token_names[*texture_idx];

                    Entity enity{getCurrentScene().get()};
                    enity.assign<Transform>(vec3(g_cursor_pos, 0.f));
                    enity.assign<Token>(size, color, name, texture, hidden);
                    enity.assign<Selectable>(0.5f * TOKEN_SIZE_VALUES[size]);
                }
                imgui::EndTabItem();
            }
            if(imgui::BeginTabItem("AreaMaker")) {
                imgui::ColorEdit3("Color", glm::value_ptr(color));
                auto shape = imgui::AreaShapeComboBox();
                imgui::SetNextItemWidth(140);
                imgui::DragDistance("##Size", &size, 0.f, 3.f);
                imgui::SameLine();
                imgui::SetNextItemWidth(60);
                imgui::UnitComboBox("Size");
                imgui::Checkbox("Hidden", &(hidden));
                imgui::SameLine();
                imgui::Checkbox("Block Light", &(block_light));

                if(imgui::Button("Create")) {
                    imgui::CloseCurrentPopup();

                    Entity enity{getCurrentScene().get()};
                    enity.assign<Transform>(vec3(g_cursor_pos, 0.f));
                    enity.assign<AreaMarker>(shape, size, color, hidden, block_light);
                    enity.assign<Selectable>(0.5f * size);
                }
                imgui::EndTabItem();
            }
            imgui::EndTabBar();
        }

        // imgui::SameLine();
        // imgui::Checkbox("Hidden", &hidden);
        // imgui::SameLine();
        // imgui::Checkbox("Emit Light", &emit_light);
    }

    void InitativeTraker(In<Camera*> primary_camera, In<Camera*> secondary_camera) {
        if(imgui::Begin("Initative Traker", &g_show_initative_traker, 0)) {
            static bool auto_focus = true;
            imgui::Checkbox("AutoFocus", &auto_focus);
            imgui::Separator();
            auto entities = getInitiativeOrder(g_maps[g_current_map].scene.get());
            uint selected = entities.size();
            for(uint i = 0; i < entities.size(); ++i) {
                if(entities[i].get<Initiative>()->active) selected = i;
            }
            for(uint i = 0; i < entities.size(); ++i) {
                auto*       initiative = entities[i].get<Initiative>();
                auto*       token      = entities[i].get<Token>();
                std::string name       = std::format(
                    "{} {:2} {}##{}", token->hidden ? "-" : "0", initiative->roll, token->name, i
                );
                if(ImGui::Selectable(name.c_str(), selected == i)) {
                    selected = i;
                    auto pos = entities[i].get<Transform>()->position;
                    primary_camera->setPosition(vec3(pos.x, pos.y, 0));
                    if(auto_focus) secondary_camera->setPosition(vec3(pos.x, pos.y, 0));
                }
            }
            for(uint i = 0; i < entities.size(); ++i) {
                entities[i].get<Initiative>()->active = selected == i;
            }
            imgui::Separator();
            if(selected < entities.size()) {
                auto* initiative = entities[selected].get<Initiative>();
                auto* token      = entities[selected].get<Token>();

                imgui::Image(
                    (void*)token->texture->getTextureHandle(),
                    imgui::ImVec2(120, 120),
                    imgui::ImVec2(0.0f, 1.0f),
                    imgui::ImVec2(1.0f, 0.0f)
                );
                imgui::ColorEdit3("##Color", glm::value_ptr(token->color));
                imgui::Checkbox("Hidden", &(token->hidden));
                imgui::SeparatorText("Light");
                bool emit_light = entities[selected].has<Light>();
                if(imgui::Checkbox("Emit Light", &emit_light)) {
                    if(emit_light) {
                        entities[selected].assign<Light>(vec2(0.f, 0.f));
                    } else {
                        entities[selected].remove<Light>();
                    }
                }
                if(auto* light = entities[selected].get<Light>())
                    imgui::DragLightRange(light->range);
                imgui::Separator();
                if(imgui::Button("Remove")) //
                    entities[selected].remove<Initiative>();
            }
        }
        imgui::End();
    }

} // namespace imgui

export void drawUI(In<Camera*> primary_camera, In<Camera*> secondary_camera) {
    constexpr char const* CONTEXT_MENU = "##CONTEXT_MENU";

    if(g_open_context_menu) {
        imgui::OpenPopup(CONTEXT_MENU);
        g_open_context_menu = false;
    }

    if(g_show_initative_traker) imgui::InitativeTraker(primary_camera, secondary_camera);

    auto& style = imgui::GetStyle();

    imgui::ImVec2 center = imgui::GetMainViewport()->GetCenter();
    imgui::SetNextWindowPos(center, imgui::ImGuiCond_Appearing, imgui::ImVec2(0.5f, 0.5f));
    bool opened = true;
    if(!imgui::BeginPopupModal(CONTEXT_MENU, &opened, imgui::ImGuiWindowFlags_AlwaysAutoResize))
        return;

    if(auto selected_entity = getSelection()) {
        imgui::EditDialog(*selected_entity);
    } else {
        imgui::CreateEntityDialog();
    }

    imgui::EndPopup();
}

export void drawMainMenu(GraphicSystem* graphics_sys) {
    if(!imgui::BeginMenuBar()) return;

    if(imgui::BeginMenu("Maps")) {
        for(uint i = 0; i < g_maps.size(); ++i) {
            auto name = g_maps[i].file.stem().string();
            if(imgui::MenuItem(name.c_str(), nullptr, false, i != g_current_map)) {
                for(auto& pipline : graphics_sys->getRenderingPipelines()) {
                    pipline->setScene(g_maps[i].scene);
                }
                g_current_map = i;
            }
        }
        imgui::Separator();
        if(imgui::MenuItem("Save Current Map State", "Ctrl+S")) { //
            g_maps[g_current_map].save();
        }
        imgui::EndMenu();
    }
    if(imgui::BeginMenu("Views")) {
        imgui::MenuItem("Initative Traker", "", &g_show_initative_traker);
        imgui::EndMenu();
    }
    imgui::EndMenuBar();
}

} // namespace bembel::examples::gm_helper