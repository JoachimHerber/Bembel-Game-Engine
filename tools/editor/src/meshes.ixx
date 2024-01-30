module;
#include <assimp/scene.h>

#include <assimp/Importer.hpp>
#include <memory>
#include <vector>
#include <string_view>
export module bembel.tools.editor:Meshes;

import bembel;

namespace bembel::tools {
using namespace bembel::base;
using namespace bembel::gui;
using namespace bembel::graphics;

export struct MeshData {
    std::string         name;
    Asset<GeometryMesh> asset;

    std::vector<graphics::DefaultVertexFormat> vertices;
    std::vector<uint>                          indices;
    struct SubMesh {
        std::string   name;
        uint          first_index;
        uint          num_indices;
    };
    std::vector<SubMesh> sub_meshes;
};

export std::vector<MeshData> MESHES;

export void drawMeshManagementUI();

export bool parseMesh(aiMesh const* ai_mat);

} // namespace bembel::tools