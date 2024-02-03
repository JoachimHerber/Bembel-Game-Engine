module;
#include <assimp/postprocess.h> // Post processing flags
#include <assimp/scene.h>

#include <assimp/Importer.hpp>
#include <memory>
#include <string_view>
module bembel.tools.editor;

import bembel;
import :Models;
import :Meshes;
import :Materials;
import :Scenes;

namespace bembel::tools {
using namespace bembel::base;
using namespace bembel::gui;
using namespace bembel::imgui;

void drawModelManagementUI() {
    if(MODELS.empty()) return;
    // m_model_preview.assign<Geometry>(m_models[index]);

    static uint selected_model = 0;
    if(imgui::BeginListBox("##ModelSelection", ImVec2(-FLT_MIN, 5 * imgui::GetTextLineHeightWithSpacing()))) {
        for(uint i = 0; i < MODELS.size(); ++i) {
            const bool is_selected = (i == selected_model);
            if(imgui::Selectable((char const*)MODELS[i].name.c_str(), is_selected)) {
                selected_model = i;
                MODEL_PREVIEW_ENTITY.assign<Geometry>(MODELS[i].asset);
            }

            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
            if(is_selected) imgui::SetItemDefaultFocus();
        }
        imgui::EndListBox();
    }
}

void parseModels(aiMesh const* ai_mat, uint material_index_offset) {
    uint mesh_index = MESHES.size();

    if(!parseMesh(ai_mat)) return;

    MODELS.emplace_back(
        ai_mat->mName.C_Str(), std::make_unique<GeometryModel>(MESHES[mesh_index].asset), mesh_index
    );
    auto& model = MODELS.back();
    model.asset->setMaterialMapping(
        ai_mat->mName.C_Str(), MATERIALS[material_index_offset + ai_mat->mMaterialIndex].asset
    );
}

} // namespace bembel::tools