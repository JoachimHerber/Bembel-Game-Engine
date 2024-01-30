module;
#include <assimp/scene.h>

#include <assimp/Importer.hpp>
#include <memory>
#include <string_view>
#include <unordered_map>
export module bembel.tools.editor:Models;

import bembel;

namespace bembel::tools {
using namespace bembel::base;
using namespace bembel::gui;
using namespace bembel::graphics;

export struct ModelData {
    std::string          name;
    Asset<GeometryModel> asset;

    uint                           mesh_index;
    std::unordered_map<uint, uint> material_mapping;
};

export std::vector<ModelData> MODELS;

export void drawModelManagementUI();

export void parseModels(aiMesh const* ai_mat, uint material_index_offset);

} // namespace bembel::tools