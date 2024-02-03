module;
#include <assimp/postprocess.h> // Post processing flags
#include <assimp/scene.h>       // Output data structure

#include <assimp/Importer.hpp> // C++ importer interface
#include <format>
#include <memory>
#include <numbers>
#include <optional>
#include <string_view>
#include <cassert>
module bembel.tools.editor;

import bembel;
import :Materials;
import :Textures;

namespace bembel::tools {
using namespace bembel::base;
using namespace bembel::gui;
using namespace bembel::imgui;

Asset<GeometryModel> initMaterialPreviewModel() {
    if(MATERIALS.empty()) {
        MATERIALS.emplace_back("Dummy", std::make_unique<Material>());
        MATERIALS.back().asset->setBaseColor(vec3(1.f, 0.f, 1.f));
        MATERIALS.back().asset->setRoughness(0.25f);
        MATERIALS.back().asset->setMetallic(0.f);
        MATERIALS.back().asset->setF0(0.004f);
    }

    std::vector<graphics::DefaultVertexFormat> vertices;
    std::vector<uint>                          indices;

    static constexpr int vertical_subdivisions   = 24;
    static constexpr int horizontal_subdivisions = 2 * vertical_subdivisions;

    for(int i = 0; i <= vertical_subdivisions; ++i) {
        float const v     = i / float(vertical_subdivisions);
        quat const  pitch = glm::angleAxis(-std::numbers::pi_v<float> * (v - 0.5f), vec3(1, 0, 0));

        for(int j = 0; j <= horizontal_subdivisions; ++j) {
            float const u   = j / float(horizontal_subdivisions);
            quat const  yaw = glm::angleAxis(std::numbers::pi_v<float> * (2 * u), vec3(0, 1, 0));

            mat3 const rot = mat3_cast(yaw * pitch);

            vertices.emplace_back(
                rot * vec3(0, 0, 1), // Pos
                rot * vec3(0, 0, 1), // Normal
                rot * vec3(1, 0, 0), // Tangent
                vec2{u, v}           // TexCoords
            );
        }
    }
    for(uint i = 0; i < vertical_subdivisions; ++i) {
        for(uint j = 0; j < horizontal_subdivisions; ++j) {
            if(i + 1 < vertical_subdivisions) {
                indices.push_back(j + i * (horizontal_subdivisions + 1));
                indices.push_back(
                    j + i * (horizontal_subdivisions + 1) + horizontal_subdivisions + 2
                );
                indices.push_back(
                    j + i * (horizontal_subdivisions + 1) + horizontal_subdivisions + 1
                );
            }
            if(i > 0) {
                indices.push_back(j + i * (horizontal_subdivisions + 1) + 0);
                indices.push_back(j + i * (horizontal_subdivisions + 1) + 1);
                indices.push_back(
                    j + i * (horizontal_subdivisions + 1) + horizontal_subdivisions + 2
                );
            }
        }
    }

    Asset<GeometryMesh> mesh = {std::make_unique<GeometryMesh>(vertices, indices)};
    mesh->setSubMesh("sphere", 0, indices.size());

    Asset<GeometryModel> model = {std::make_unique<GeometryModel>(mesh)};
    model->setMaterialMapping("sphere", MATERIALS[0].asset);

    MATERIAL_PREVIEW_MODEL = model.getHndl();
    return model;
}

void getTexture(
    In<aiMaterial const*> ai_mat, In<aiTextureType> type, InOut<std::optional<uint>> index
) {
    if(ai_mat->GetTextureCount(type) == 0) return;

    aiString ai_name;
    if(ai_mat->GetTexture(type, 0, &ai_name, NULL, NULL, NULL, NULL, NULL) != AI_SUCCESS) return;
    std::string name = ai_name.C_Str();

    for(uint i = 0; i < TEXTURES.size(); ++i) {
        if(TEXTURES[i].name == name) {
            index = i;
            return;
        }
    }

    Asset<Texture> texture;
    texture.request(name);
    if(texture) {
        index = TEXTURES.size();
        TEXTURES.emplace_back(name, texture);
    }
};

void parseMaterial(aiMaterial const* ai_mat) {
    assert(ai_mat);

    MATERIALS.emplace_back(ai_mat->GetName().C_Str(), std::make_unique<Material>());

    MaterialData& mat = MATERIALS.back();

    // clang-format off
    getTexture( ai_mat, aiTextureType_DIFFUSE,           mat.base_color_texture_index     );
    getTexture( ai_mat, aiTextureType_SPECULAR,          mat.material_props_texture_index );
    getTexture( ai_mat, aiTextureType_EMISSIVE,          mat.emission_texture_index       );
    getTexture( ai_mat, aiTextureType_HEIGHT,            mat.normal_map_texture_index     );
    getTexture( ai_mat, aiTextureType_NORMALS,           mat.normal_map_texture_index     );
    getTexture( ai_mat, aiTextureType_SHININESS,         mat.material_props_texture_index );
    getTexture( ai_mat, aiTextureType_DISPLACEMENT,      mat.normal_map_texture_index     );
    getTexture( ai_mat, aiTextureType_BASE_COLOR,        mat.base_color_texture_index     );
    getTexture( ai_mat, aiTextureType_NORMAL_CAMERA,     mat.normal_map_texture_index     );
    getTexture( ai_mat, aiTextureType_EMISSION_COLOR,    mat.emission_texture_index       );
    getTexture( ai_mat, aiTextureType_METALNESS,         mat.material_props_texture_index );
    getTexture( ai_mat, aiTextureType_DIFFUSE_ROUGHNESS, mat.base_color_texture_index     );
    // clang-format on

    // logInfo("Mat: {}", ai_mat->GetName().C_Str());
    // for(auto* prop : std::span(ai_mat->mProperties, ai_mat->mNumProperties)) {
    //     switch(prop->mType) {
    //         case aiPropertyTypeInfo::aiPTI_Float:   logInfo("[{}]: Float[{}]",
    //         prop->mKey.C_Str(), prop->mDataLength / sizeof(float)  ); break; case
    //         aiPropertyTypeInfo::aiPTI_Double:  logInfo("[{}]: Double[{}]",
    //         prop->mKey.C_Str(), prop->mDataLength / sizeof(double) ); break; case
    //         aiPropertyTypeInfo::aiPTI_String:  logInfo("[{}]: String[{}]",
    //         prop->mKey.C_Str(), prop->mDataLength                  ); break; case
    //         aiPropertyTypeInfo::aiPTI_Integer: logInfo("[{}]: Integer[{}]",
    //         prop->mKey.C_Str(), prop->mDataLength / sizeof(int)    ); break; case
    //         aiPropertyTypeInfo::aiPTI_Buffer:  logInfo("[{}]: Buffer[{}]",
    //         prop->mKey.C_Str(), prop->mDataLength                  ); break;
    //     }
    // }

    if(mat.emission_texture_index) {
        mat.asset->setEmissionTexture(TEXTURES[*mat.emission_texture_index].asset);
        mat.asset->setEmission(vec3(1.f, 1.f, 1.f));
    }
    if(mat.base_color_texture_index) {
        mat.asset->setBaseColorTexture(TEXTURES[*mat.base_color_texture_index].asset);
        mat.asset->setBaseColor(vec3(1.f, 1.f, 1.f));
    }
    if(mat.material_props_texture_index) {
        mat.asset->setMaterialPropsTexture(TEXTURES[*mat.material_props_texture_index].asset);
        mat.asset->setRoughness(1.f);
        mat.asset->setMetallic(1.f);
        mat.asset->setF0(1.f);
    }
    if(mat.normal_map_texture_index) {
        mat.asset->setNormalMapTexture(TEXTURES[*mat.normal_map_texture_index].asset);
    }

    aiColor3D color;
    float     value;
    if(ai_mat->Get(AI_MATKEY_COLOR_EMISSIVE, color) == AI_SUCCESS) {
        mat.asset->setEmission(vec3(color.r, color.g, color.b));
    }
    if(ai_mat->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS
       || ai_mat->Get(AI_MATKEY_BASE_COLOR, color) == AI_SUCCESS) {
        mat.asset->setBaseColor(vec3(color.r, color.g, color.b));
    }
    if(ai_mat->Get(AI_MATKEY_ROUGHNESS_FACTOR, value) == AI_SUCCESS) { //
        mat.asset->setRoughness(value);
    }
    if(ai_mat->Get(AI_MATKEY_METALLIC_FACTOR, value) == AI_SUCCESS) { //
        mat.asset->setMetallic(value);
    }
    if(ai_mat->Get(AI_MATKEY_COLOR_SPECULAR, color) == AI_SUCCESS) {
        mat.asset->setF0((color.r + color.g + color.b) / 3.f);
    }
}

void drawMaterialManagementUI() {
    static uint selected_material = 0;

    if(MATERIALS.empty()) return;

    if(!imgui::BeginListBox(
           "Material", ImVec2(-FLT_MIN, 5 * imgui::GetTextLineHeightWithSpacing())
       )) {
        return;
    }
    for(uint i = 0; i < MATERIALS.size(); ++i) {
        const bool is_selected = (selected_material == i);
        if(imgui::Selectable((char const*)MATERIALS[i].name.c_str(), is_selected)) {
            selected_material = i;

            Asset<GeometryModel> model = MATERIAL_PREVIEW_MODEL;
            model->setMaterialMapping("sphere", MATERIALS[i].asset);
        }

        // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
        if(is_selected) imgui::SetItemDefaultFocus();
    }
    imgui::EndListBox();

    drawMaterialEditUI(MATERIALS[selected_material]);
}

void drawMaterialEditUI(MaterialData& mat_data) {
    imgui::SeparatorText("Material Properties");

    vec3  color;
    float value;

    auto* mat = mat_data.asset.get();

    color = mat->getEmission();
    if(imgui::ColorEdit3("Emission", &color.r)) mat->setEmission(color);
    color = mat->getBaseColor();
    if(imgui::ColorEdit3("Base Color", &color.r)) mat->setBaseColor(color);

    value = mat->getRoughness();
    if(imgui::SliderFloat("Roughness", &value, 0.0f, 1.0f, "%.3f")) mat->setRoughness(value);
    value = mat->getMetallic();
    if(imgui::SliderFloat("Metallic", &value, 0.0f, 1.0f, "%.3f")) mat->setMetallic(value);
    value = mat->getF0();
    if(imgui::SliderFloat("F0", &value, 0.0f, 1.0f, "%.3f")) mat->setF0(value);

    imgui::SeparatorText("Textures");

    if(drawTextureSelectionUI("Emission", mat_data.emission_texture_index)) {
        if(mat_data.emission_texture_index) {
            mat->setEmissionTexture(TEXTURES[*mat_data.emission_texture_index].asset);
        } else {
            mat->setEmissionTexture({});
        }
    }
    if(drawTextureSelectionUI("Base Color", mat_data.base_color_texture_index)) {
        if(mat_data.base_color_texture_index) {
            mat->setBaseColorTexture(TEXTURES[*mat_data.base_color_texture_index].asset);
        } else {
            mat->setBaseColorTexture({});
        }
    }
    if(drawTextureSelectionUI("Roughness/Metalic/F0", mat_data.material_props_texture_index)) {
        if(mat_data.material_props_texture_index) {
            mat->setMaterialPropsTexture(TEXTURES[*mat_data.material_props_texture_index].asset);
        } else {
            mat->setMaterialPropsTexture({});
        }
    }
    if(drawTextureSelectionUI("Normal Map", mat_data.normal_map_texture_index)) {
        if(mat_data.normal_map_texture_index) {
            mat->setNormalMapTexture(TEXTURES[*mat_data.normal_map_texture_index].asset);
        } else {
            mat->setNormalMapTexture({});
        }
    }
}

} // namespace bembel::tools