module;
#include <assimp/postprocess.h> // Post processing flags
#include <assimp/scene.h>

#include <assimp/Importer.hpp>
#include <memory>
#include <string_view>
#include <cassert>
module bembel.tools.editor;

import bembel;
import :Meshes;

namespace bembel::tools {
using namespace bembel::base;
using namespace bembel::gui;
using namespace bembel::imgui;

bool parseMesh(aiMesh const* ai_mesh) {
    assert(ai_mesh);

    if(!ai_mesh->HasFaces()) {
        logError("Mesh '{}' has no Faces", ai_mesh->mName.C_Str());
        return false;
    }
    if(!ai_mesh->HasNormals()) {
        logError("Mesh '{}' has no Normals", ai_mesh->mName.C_Str());
        return false;
    }
    if(!ai_mesh->HasPositions()) {
        logError("Mesh '{}' has no Positions", ai_mesh->mName.C_Str());
        return false;
    }
    if(!ai_mesh->HasTangentsAndBitangents()) {
        logError("Mesh '{}' has no TangentsAndBitangents", ai_mesh->mName.C_Str());
        return false;
    }
    if(!ai_mesh->HasTextureCoords(0)) {
        logError("Mesh '{}' has no Texture Coords", ai_mesh->mName.C_Str());
        return false;
    }
    if(ai_mesh->HasBones()) return false;

    MESHES.emplace_back(ai_mesh->mName.C_Str());

    auto& mesh = MESHES.back();

    for(auto i = 0; i < ai_mesh->mNumVertices; ++i) {
        auto& pos        = ai_mesh->mVertices[i];
        auto& normal     = ai_mesh->mNormals[i];
        auto& tangent    = ai_mesh->mTangents[i];
        auto& bitangent  = ai_mesh->mBitangents[i];
        auto& tex_coords = ai_mesh->mTextureCoords[0][i];

        mesh.vertices.emplace_back(
            vec3(pos.x, pos.y, pos.z),
            vec3(normal.x, normal.y, normal.z),
            vec3(tangent.x, tangent.y, tangent.z),
            vec2(tex_coords.x, tex_coords.y)
        );
    }

    for(auto& face : std::span(ai_mesh->mFaces, ai_mesh->mNumFaces)) {
        if(face.mNumIndices != 3) {
            logError("Mesh '{}' has non triangular faces", ai_mesh->mName.C_Str());
            return false;
        }
        mesh.indices.push_back(face.mIndices[0]);
        mesh.indices.push_back(face.mIndices[1]);
        mesh.indices.push_back(face.mIndices[2]);
    }
    mesh.sub_meshes.emplace_back(mesh.name, 0, mesh.indices.size());

    mesh.asset.asign(std::make_unique<GeometryMesh>(mesh.vertices, mesh.indices));
    mesh.asset->setSubMesh(mesh.name, 0, mesh.indices.size());

    return true;
}

void drawMeshManagementUI() {
    static uint selected_mesh = 0;

    if(MESHES.empty()) return;
    // m_model_preview.assign<Geometry>(m_models[index]);

    if(imgui::BeginListBox(
           "##MeshSelection", ImVec2(-FLT_MIN, 5 * imgui::GetTextLineHeightWithSpacing())
       )) {
        for(uint i = 0; i < MESHES.size(); ++i) {
            const bool is_selected = (i == selected_mesh);
            if(imgui::Selectable(MESHES[i].name.c_str(), is_selected)) selected_mesh = i;

            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
            if(is_selected) imgui::SetItemDefaultFocus();
        }
        imgui::EndListBox();
    }
}

} // namespace bembel::tools