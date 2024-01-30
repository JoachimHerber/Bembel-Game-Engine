module;
#include <assimp/scene.h>

#include <array>
#include <assimp/Importer.hpp>
#include <memory>
#include <optional>
#include <string_view>
export module bembel.tools.editor:Materials;

import bembel;

namespace bembel::tools {
using namespace bembel::base;
using namespace bembel::gui;
using namespace bembel::graphics;

export struct MaterialData {
    std::string     name;
    Asset<Material> asset;

    std::optional<uint> emission_texture_index;
    std::optional<uint> base_color_texture_index;
    std::optional<uint> material_props_texture_index;
    std::optional<uint> normal_map_texture_index;
};

export std::vector<MaterialData> MATERIALS;

export AssetHandle MATERIAL_PREVIEW_MODEL;

export Asset<GeometryModel> initMaterialPreviewModel();

export void drawMaterialManagementUI();
export void drawMaterialEditUI(MaterialData&);

export void parseMaterial(aiMaterial const* ai_mat);

} // namespace bembel::tools