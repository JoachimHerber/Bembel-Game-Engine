module;
#include <assimp/postprocess.h> // Post processing flags
#include <assimp/scene.h>

#include <assimp/Importer.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <memory>
#include <string_view>
module bembel.tools.editor;

import bembel;

namespace bembel::tools {
using namespace bembel::base;
using namespace bembel::gui;

ModelManager::ModelManager(
    In<TabGroupWidget::Tab*> tab, In<RenderingPipeline*> pipeline, In<MaterialManager*> materials
)
  : m_tab{tab}, m_pipeline{pipeline}, m_materials{materials} {
    using LinearWidgetLayout::Mode::SCALE_TO_FIT;

    m_tab->select_signal.bind(this, &ModelManager::onTabSelected);

    m_scene = std::make_shared<Scene>();
    m_scene->registerComponentType<DirectionalLight>();
    m_scene->registerComponentType<Transform>();
    m_scene->registerComponentType<Geometry>();

    Entity ligth0 = {*m_scene, m_scene->createEntity()};
    Entity ligth1 = {*m_scene, m_scene->createEntity()};
    Entity ligth2 = {*m_scene, m_scene->createEntity()};

    ligth0.assign<DirectionalLight>(vec3(5.0f), glm::normalize(glm::vec3(-0.3, -1, -0.2)), false);
    ligth1.assign<DirectionalLight>(vec3(3.0f), glm::normalize(glm::vec3(0.7, -1, 0.0)), false);
    ligth2.assign<DirectionalLight>(vec3(1.0f), glm::normalize(glm::vec3(0.3, -1, +0.6)), false);

    m_model_preview = Entity{*m_scene, m_scene->createEntity()};
    m_model_preview.assign<Transform>(vec3(0.f, 0.f, 0.f));

    auto& menu  = m_tab->getContentArea();
    auto  style          = menu.getStyle();
    auto  label_0        = menu.createChildWidget<LabelWidget>(u8"Models:");
    m_model_selection    = menu.createChildWidget<RadioButtonGroupWidget>();

    m_model_selection->selection_change_signal.bind(this, &ModelManager::onModelSelection);

    auto layout = menu.setLayout<LinearWidgetLayout>(SCALE_TO_FIT);
    layout->setMargin(8, 8);
    layout->addRow({.min_height = 8, .rel_height = 0.0});
    layout->addRow().addWidget(label_0);
    layout->addRow().addWidget(m_model_selection);
    layout->addRow({.min_height = 0, .rel_height = 1.0});

    menu.updateLayout();
    menu.updateLayout();
}

void ModelManager::parseScene(aiScene const* ai_scene) {
    std::vector<graphics::DefaultVertexFormat> vertices;
    std::vector<uint>                          indices;
    struct SubMesh {
        aiMesh const* ai_mesh;
        uint          first_index;
        uint          num_indices;
    };
    std::vector<SubMesh> sub_meshes;

    for(auto ai_mesh : std::span(ai_scene->mMeshes, ai_scene->mNumMeshes)) {
        if(ai_mesh->HasBones()) continue;

        if(!ai_mesh->HasFaces()) return logError("Mesh '{}' has no Faces", ai_mesh->mName.C_Str());
        if(!ai_mesh->HasNormals())
            return logError("Mesh '{}' has no Normals", ai_mesh->mName.C_Str());
        if(!ai_mesh->HasPositions())
            return logError("Mesh '{}' has no Positions", ai_mesh->mName.C_Str());
        if(!ai_mesh->HasTangentsAndBitangents())
            return logError("Mesh '{}' has no TangentsAndBitangents", ai_mesh->mName.C_Str());
        if(!ai_mesh->HasTextureCoords(0))
            return logError("Mesh '{}' has no Texture Coords", ai_mesh->mName.C_Str());

        uint start_index = vertices.size();
        for(auto i = 0; i < ai_mesh->mNumVertices; ++i) {
            auto& pos        = ai_mesh->mVertices[i];
            auto& normal     = ai_mesh->mNormals[i];
            auto& tangent    = ai_mesh->mTangents[i];
            auto& bitangent  = ai_mesh->mBitangents[i];
            auto& tex_coords = ai_mesh->mTextureCoords[0][i];

            mat3 normal_transform{
                vec3(tangent.x, tangent.y, tangent.z),
                vec3(bitangent.x, bitangent.y, bitangent.z),
                vec3(normal.x, normal.y, normal.z)};

            vertices.emplace_back(
                vec3(pos.x, pos.y, pos.z),
                glm::quat_cast(normal_transform),
                vec2(tex_coords.x, tex_coords.y)
            );
        }
        uint sub_mesh_begin = indices.size();
        for(auto& face : std::span(ai_mesh->mFaces, ai_mesh->mNumFaces)) {
            if(face.mNumIndices != 3)
                return logError("Mesh '{}' has non triangular faces", ai_mesh->mName.C_Str());
            indices.push_back(start_index + face.mIndices[0]);
            indices.push_back(start_index + face.mIndices[1]);
            indices.push_back(start_index + face.mIndices[2]);
        }
        sub_meshes.emplace_back(ai_mesh, sub_mesh_begin, indices.size() - sub_mesh_begin);
    }
    Asset<GeometryMesh> mesh;
    mesh.init(std::make_unique<GeometryMesh>(vertices, indices));
    for(auto& sub_mesh : sub_meshes) {
        mesh->setSubMesh(
            sub_mesh.ai_mesh->mName.C_Str(), sub_mesh.first_index, sub_mesh.num_indices
        );

        m_models.emplace_back();
        m_models.back().init(std::make_unique<GeometryModel>(mesh));
        m_models.back()->setMaterialMapping(
          sub_mesh.ai_mesh->mName.C_Str(), 
          m_materials->getMaterial(ai_scene, sub_mesh.ai_mesh->mMaterialIndex)
        );

        m_model_selection->addRadioButton((char8_t const*)sub_mesh.ai_mesh->mName.C_Str());
    }
    m_meshes.push_back(std::move(mesh));
    m_tab->getContentArea().updateLayout();
}

void ModelManager::onTabSelected() {
    m_pipeline->setScene(m_scene);
}

void ModelManager::onModelSelection(int index){
    if(index < 0 || m_models.size() <= index) return;

    m_model_preview.assign<Geometry>(m_models[index]);
}

} // namespace bembel::tools