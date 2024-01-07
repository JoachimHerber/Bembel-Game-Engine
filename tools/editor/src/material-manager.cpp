module;
#include <assimp/postprocess.h> // Post processing flags
#include <assimp/scene.h>       // Output data structure

#include <assimp/Importer.hpp> // C++ importer interface
#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <memory>
#include <optional>
#include <string_view>
module bembel.tools.editor;

import bembel;

namespace bembel::tools {
using namespace bembel::base;
using namespace bembel::gui;

MaterialManager::MaterialManager(In<TabGroupWidget::Tab*> tab, In<RenderingPipeline*> pipeline)
  : m_tab{tab}, m_pipeline{pipeline} {
    using LinearWidgetLayout::Mode::SCALE_TO_FIT;

    m_tab->select_signal.bind(this, &MaterialManager::onTabSelected);

    m_scene = std::make_shared<Scene>();
    m_scene->registerComponentType<DirectionalLight>();
    m_scene->registerComponentType<Transform>();
    m_scene->registerComponentType<Geometry>();
    m_pipeline->setScene(m_scene);

    Entity ligth0 = {*m_scene, m_scene->createEntity()};
    Entity ligth1 = {*m_scene, m_scene->createEntity()};
    Entity ligth2 = {*m_scene, m_scene->createEntity()};

    ligth0.assign<DirectionalLight>(vec3(5.0f), glm::normalize(glm::vec3(-0.3, -1, -0.2)), true);
    ligth1.assign<DirectionalLight>(vec3(3.0f), glm::normalize(glm::vec3(0.7, -1, 0.0)), true);
    ligth2.assign<DirectionalLight>(vec3(1.0f), glm::normalize(glm::vec3(0.3, -1, +0.6)), true);

    m_model_preview = Entity{*m_scene, m_scene->createEntity()};
    m_model_preview.assign<Transform>(vec3(0.f, 0.f, 0.f));

    auto& menu  = m_tab->getContentArea();
    auto  style = menu.getStyle();

    auto label_0         = menu.createChildWidget<LabelWidget>(u8"Materials:");
    m_material_selection = menu.createChildWidget<RadioButtonGroupWidget>();
    auto seperartor_0    = menu.createChildWidget<GroupWidget>();

    auto label_1 = menu.createChildWidget<LabelWidget>(u8"Emission:");
    auto label_2 = menu.createChildWidget<LabelWidget>(u8"Base Color:");

    auto label_3 = menu.createChildWidget<LabelWidget>(u8"Roughness:");
    auto label_4 = menu.createChildWidget<LabelWidget>(u8"Metalic:");
    auto label_5 = menu.createChildWidget<LabelWidget>(u8"F0:");

    m_slider_widgets[0] = menu.createChildWidget<IntSliderWidget>(0, 255);
    m_slider_widgets[1] = menu.createChildWidget<IntSliderWidget>(0, 255);
    m_slider_widgets[2] = menu.createChildWidget<IntSliderWidget>(0, 255);
    m_slider_widgets[3] = menu.createChildWidget<IntSliderWidget>(0, 255);
    m_slider_widgets[4] = menu.createChildWidget<IntSliderWidget>(0, 255);
    m_slider_widgets[5] = menu.createChildWidget<IntSliderWidget>(0, 255);
    m_slider_widgets[6] = menu.createChildWidget<IntSliderWidget>(0, 255);
    m_slider_widgets[7] = menu.createChildWidget<IntSliderWidget>(0, 255);
    m_slider_widgets[8] = menu.createChildWidget<IntSliderWidget>(0, 255);

    m_slider_widgets[0]->value_update_signal.bind(this, &MaterialManager::onUpdateMaterialParam);
    m_slider_widgets[1]->value_update_signal.bind(this, &MaterialManager::onUpdateMaterialParam);
    m_slider_widgets[2]->value_update_signal.bind(this, &MaterialManager::onUpdateMaterialParam);
    m_slider_widgets[3]->value_update_signal.bind(this, &MaterialManager::onUpdateMaterialParam);
    m_slider_widgets[4]->value_update_signal.bind(this, &MaterialManager::onUpdateMaterialParam);
    m_slider_widgets[5]->value_update_signal.bind(this, &MaterialManager::onUpdateMaterialParam);
    m_slider_widgets[6]->value_update_signal.bind(this, &MaterialManager::onUpdateMaterialParam);
    m_slider_widgets[7]->value_update_signal.bind(this, &MaterialManager::onUpdateMaterialParam);
    m_slider_widgets[8]->value_update_signal.bind(this, &MaterialManager::onUpdateMaterialParam);

    auto label_6 = menu.createChildWidget<LabelWidget>(u8"Emission Texture:");
    auto label_7 = menu.createChildWidget<LabelWidget>(u8"Base Color Texture:");
    auto label_8 = menu.createChildWidget<LabelWidget>(u8"Roughness/Metalic/F0 Texture:");
    auto label_9 = menu.createChildWidget<LabelWidget>(u8"Normal Map Texture:");
    m_texture_path_inputs[0] = menu.createChildWidget<TextInputWidget>();
    m_texture_path_inputs[1] = menu.createChildWidget<TextInputWidget>();
    m_texture_path_inputs[2] = menu.createChildWidget<TextInputWidget>();
    m_texture_path_inputs[3] = menu.createChildWidget<TextInputWidget>();
    m_load_texture_button[0] = menu.createChildWidget<ButtonWidget>(u8"Load");
    m_load_texture_button[1] = menu.createChildWidget<ButtonWidget>(u8"Load");
    m_load_texture_button[2] = menu.createChildWidget<ButtonWidget>(u8"Load");
    m_load_texture_button[3] = menu.createChildWidget<ButtonWidget>(u8"Load");

    m_texture_path_inputs[0]->text.change_signal.bind(this, &MaterialManager::onTextUpdate);
    m_texture_path_inputs[1]->text.change_signal.bind(this, &MaterialManager::onTextUpdate);
    m_texture_path_inputs[2]->text.change_signal.bind(this, &MaterialManager::onTextUpdate);
    m_texture_path_inputs[3]->text.change_signal.bind(this, &MaterialManager::onTextUpdate);

    m_load_texture_button[0]->click_signal.bind(this, &MaterialManager::loadEmissionTexture);
    m_load_texture_button[1]->click_signal.bind(this, &MaterialManager::loadBaseColorTexture);
    m_load_texture_button[2]->click_signal.bind(this, &MaterialManager::loadMaterialPropsTexture);
    m_load_texture_button[3]->click_signal.bind(this, &MaterialManager::loadNormalMapTexture);

    seperartor_0->background_color = style->getColor(Style::Colors::WINDOW_BORDER);

    m_material_selection->selection_change_signal.bind(this, &MaterialManager::onMaterialSelection);

    auto layout = menu.setLayout<LinearWidgetLayout>(SCALE_TO_FIT);
    layout->setMargin(8, 8);
    layout->addRow({.min_height = 8, .rel_height = 0.0});
    layout->addRow().addWidget(label_0);
    layout->addRow().addWidget(m_material_selection);
    layout->addRow({.min_height = 2, .rel_height = 0.0}).addWidget(seperartor_0);
    layout->addRow({.height = 6});
    layout->addRow({.height = 28})
        .addWidget(label_1)
        .addWidget(m_slider_widgets[0], 0.f, 52)
        .addSpacing(2)
        .addWidget(m_slider_widgets[1], 0.f, 52)
        .addSpacing(2)
        .addWidget(m_slider_widgets[2], 0.f, 52);
    layout->addRow({.height = 2});
    layout->addRow({.height = 28})
        .addWidget(label_2)
        .addWidget(m_slider_widgets[3], 0.f, 52)
        .addSpacing(2)
        .addWidget(m_slider_widgets[4], 0.f, 52)
        .addSpacing(2)
        .addWidget(m_slider_widgets[5], 0.f, 52);
    layout->addRow({.height = 2});
    layout->addRow({.height = 28}).addWidget(label_3).addWidget(m_slider_widgets[6], 0.f, 160);
    layout->addRow({.height = 2});
    layout->addRow({.height = 28}).addWidget(label_4).addWidget(m_slider_widgets[7], 0.f, 160);
    layout->addRow({.height = 2});
    layout->addRow({.height = 28}).addWidget(label_5).addWidget(m_slider_widgets[8], 0.f, 160);
    layout->addRow({.height = 24}).addWidget(label_6);
    layout->addRow({.height = 28})
        .addWidget(m_texture_path_inputs[0])
        .addSpacing(2)
        .addWidget(m_load_texture_button[0], 0.0f);
    layout->addRow({.height = 24}).addWidget(label_7);
    layout->addRow({.height = 28})
        .addWidget(m_texture_path_inputs[1])
        .addSpacing(2)
        .addWidget(m_load_texture_button[1], 0.0f);
    layout->addRow({.height = 24}).addWidget(label_8);
    layout->addRow({.height = 28})
        .addWidget(m_texture_path_inputs[2])
        .addSpacing(2)
        .addWidget(m_load_texture_button[2], 0.0f);
    layout->addRow({.height = 24}).addWidget(label_9);
    layout->addRow({.height = 28})
        .addWidget(m_texture_path_inputs[3])
        .addSpacing(2)
        .addWidget(m_load_texture_button[3], 0.0f);
    layout->addRow({.min_height = 0, .rel_height = 1.0});

    menu.updateLayout();
    menu.updateLayout();

    initPreview();
}

void MaterialManager::parseScene(aiScene const* ai_scene) {
    for(uint idx = 0; idx < ai_scene->mNumMaterials; ++idx) {
        m_materials.emplace_back(ai_scene, idx);
        m_material_selection->addRadioButton(m_materials.back().name);
    }
    m_tab->getContentArea().updateLayout();
}

MaterialManager::Mat::Mat(aiScene const* _scene, uint idx) : scene{_scene}, index{idx} {
    this->material.init(std::make_unique<Material>());
    if(scene) {
        auto ai_mat = scene->mMaterials[index];
        this->name  = (char8_t const*)(ai_mat->GetName().C_Str());

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

        aiColor3D color;
        if(ai_mat->Get(AI_MATKEY_COLOR_EMISSIVE, color) == AI_SUCCESS) {
            material->setEmission(vec3(color.r, color.g, color.b));
        }
        if(ai_mat->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS
           || ai_mat->Get(AI_MATKEY_BASE_COLOR, color) == AI_SUCCESS) {
            material->setBaseColor(vec3(color.r, color.g, color.b));
        }
        float value;
        if(ai_mat->Get(AI_MATKEY_ROUGHNESS_FACTOR, value) == AI_SUCCESS) {
            material->setRoughness(value);
        }
        if(ai_mat->Get(AI_MATKEY_METALLIC_FACTOR, value) == AI_SUCCESS) {
            material->setMetallic(value);
        }
        if(ai_mat->Get(AI_MATKEY_COLOR_SPECULAR, color) == AI_SUCCESS) {
            material->setF0((color.r + color.g + color.b) / 3.f);
        }

        auto getTexture = [&](In<aiTextureType> type, Out<std::filesystem::path> path) {
            if(ai_mat->GetTextureCount(type) > 0) {
                aiString Path;
                if(ai_mat->GetTexture(type, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
                    path = Path.C_Str();
                }
            }
        };

        // clang-format off
        getTexture( aiTextureType_DIFFUSE,           base_color_texture     );
        getTexture( aiTextureType_SPECULAR,          material_props_texture );
        getTexture( aiTextureType_EMISSIVE,          emission_texture       );
        getTexture( aiTextureType_HEIGHT,            normal_map_texture     );
        getTexture( aiTextureType_NORMALS,           normal_map_texture     );
        getTexture( aiTextureType_SHININESS,         material_props_texture );
        getTexture( aiTextureType_DISPLACEMENT,      normal_map_texture     );
        getTexture( aiTextureType_BASE_COLOR,        base_color_texture     );
        getTexture( aiTextureType_NORMAL_CAMERA,     normal_map_texture     );
        getTexture( aiTextureType_EMISSION_COLOR,    emission_texture       );
        getTexture( aiTextureType_METALNESS,         material_props_texture );
        getTexture( aiTextureType_DIFFUSE_ROUGHNESS, base_color_texture     );
        // clang-format on

        if(!emission_texture.empty()) {
            Asset<Texture> texture;
            if(texture.request(emission_texture.string())) {
                material->setEmissionTexture(texture);
            }
        }
        if(!base_color_texture.empty()) {
            Asset<Texture> texture;
            if(texture.request(base_color_texture.string())) {
                material->setBaseColorTexture(texture);
            }
        }
        if(!material_props_texture.empty()) {
            Asset<Texture> texture;
            if(texture.request(material_props_texture.string())) {
                material->setMaterialPropsTexture(texture);
            }
        }
        if(!normal_map_texture.empty()) {
            Asset<Texture> texture;
            if(texture.request(normal_map_texture.string())) {
                material->setNormalMapTexture(texture);
            }
        }
    }
}

void MaterialManager::onTabSelected() {
    m_pipeline->setScene(m_scene);
}

void MaterialManager::onMaterialSelection(int index) {
    if(index < 0 || m_materials.size() <= index) return;

    auto& mat = m_materials[index];

    m_preview_model->setMaterialMapping("sphere", mat.material);

    m_texture_path_inputs[0]->text = mat.emission_texture.u8string();
    m_texture_path_inputs[1]->text = mat.base_color_texture.u8string();
    m_texture_path_inputs[2]->text = mat.material_props_texture.u8string();
    m_texture_path_inputs[3]->text = mat.normal_map_texture.u8string();
    m_load_texture_button[0]->disable();
    m_load_texture_button[1]->disable();
    m_load_texture_button[2]->disable();
    m_load_texture_button[3]->disable();

    auto* m = mat.material.get();

    auto emission   = m->getEmission();
    auto base_color = m->getBaseColor();
    auto roughness  = m->getRoughness();
    auto metallic   = m->getMetallic();
    auto f0         = m->getF0();

    m_slider_widgets[0]->setValue(255 * emission.r);
    m_slider_widgets[1]->setValue(255 * emission.g);
    m_slider_widgets[2]->setValue(255 * emission.b);
    m_slider_widgets[3]->setValue(255 * base_color.r);
    m_slider_widgets[4]->setValue(255 * base_color.g);
    m_slider_widgets[5]->setValue(255 * base_color.b);
    m_slider_widgets[6]->setValue(255 * roughness);
    m_slider_widgets[7]->setValue(255 * metallic);
    m_slider_widgets[8]->setValue(255 * f0);
}

void MaterialManager::onUpdateMaterialParam(In<i64>) {
    auto index = m_material_selection->getSelection();
    if(index < 0 || m_materials.size() <= index) return;

    auto* mat = m_materials[index].material.get();

    mat->setEmission(vec3(
        m_slider_widgets[0]->getValue() / 255.f,
        m_slider_widgets[1]->getValue() / 255.f,
        m_slider_widgets[2]->getValue() / 255.f
    ));
    mat->setBaseColor(vec3(
        m_slider_widgets[3]->getValue() / 255.f,
        m_slider_widgets[4]->getValue() / 255.f,
        m_slider_widgets[5]->getValue() / 255.f
    ));
    mat->setRoughness(m_slider_widgets[6]->getValue() / 255.f);
    mat->setMetallic(m_slider_widgets[7]->getValue() / 255.f);
    mat->setF0(m_slider_widgets[8]->getValue() / 255.f);
}

void MaterialManager::onTextUpdate(In<std::u8string>, In<std::u8string>) {
    auto index = m_material_selection->getSelection();
    if(index < 0 || m_materials.size() <= index) return;

    auto& mat = m_materials[index];
    if(mat.emission_texture == m_texture_path_inputs[0]->text.get()) {
        m_load_texture_button[0]->disable();
    } else {
        m_load_texture_button[0]->enable();
    }
    if(mat.base_color_texture == m_texture_path_inputs[1]->text.get()) {
        m_load_texture_button[1]->disable();
    } else {
        m_load_texture_button[1]->enable();
    }
    if(mat.material_props_texture == m_texture_path_inputs[2]->text.get()) {
        m_load_texture_button[2]->disable();
    } else {
        m_load_texture_button[2]->enable();
    }
    if(mat.normal_map_texture == m_texture_path_inputs[3]->text.get()) {
        m_load_texture_button[3]->disable();
    } else {
        m_load_texture_button[3]->enable();
    }
}

void MaterialManager::loadEmissionTexture() {
    auto index = m_material_selection->getSelection();
    if(index < 0 || m_materials.size() <= index) return;

    auto& mat = m_materials[index];

    Asset<Texture>        texture;
    std::filesystem::path path = m_texture_path_inputs[0]->text.get();
    if(texture.request(path.string())) {
        mat.material->setEmissionTexture(texture);
        mat.emission_texture = path;
        m_load_texture_button[0]->disable();
    }
}

void MaterialManager::loadBaseColorTexture() {
    auto index = m_material_selection->getSelection();
    if(index < 0 || m_materials.size() <= index) return;

    auto& mat = m_materials[index];

    Asset<Texture>        texture;
    std::filesystem::path path = m_texture_path_inputs[1]->text.get();
    if(texture.request(path.string())) {
        mat.material->setBaseColorTexture(texture);
        mat.base_color_texture = path;
        m_load_texture_button[1]->disable();
    }
}

void MaterialManager::loadMaterialPropsTexture() {
    auto index = m_material_selection->getSelection();
    if(index < 0 || m_materials.size() <= index) return;

    auto& mat = m_materials[index];

    Asset<Texture>        texture;
    std::filesystem::path path = m_texture_path_inputs[2]->text.get();
    if(texture.request(path.string())) {
        mat.material->setMaterialPropsTexture(texture);
        mat.material_props_texture = path;
        m_load_texture_button[2]->disable();
    }
}
void MaterialManager::loadNormalMapTexture() {
    auto index = m_material_selection->getSelection();
    if(index < 0 || m_materials.size() <= index) return;

    auto& mat = m_materials[index];

    Asset<Texture>        texture;
    std::filesystem::path path = m_texture_path_inputs[3]->text.get();
    if(texture.request(path.string())) {
        mat.material->setNormalMapTexture(texture);
        mat.normal_map_texture = path;
        m_load_texture_button[3]->disable();
    }
}

void MaterialManager::initPreview() {
    m_materials.emplace_back(nullptr, 0);

    m_materials[0].material->setEmission({0.f, 0.f, 0.f});
    m_materials[0].material->setBaseColor({1.f, 0.f, 1.f});
    m_materials[0].material->setRoughness(0.1f);
    m_materials[0].material->setMetallic(0.1f);
    m_materials[0].material->setF0(0.04);

    std::vector<graphics::DefaultVertexFormat> vertices;
    std::vector<uint>                          indices;

    for(int i = 0; i <= 12; ++i) {
        float const v     = i / 12.f;
        quat const  pitch = glm::angleAxis(-3.14159265359f * (v - 0.5f), vec3(1, 0, 0));

        for(int j = 0; j <= 24; ++j) {
            float const u   = j / 24.f;
            quat const  yaw = glm::angleAxis(3.14159265359f * (2 * u), vec3(0, 1, 0));

            quat const rot = yaw * pitch;

            vertices.emplace_back(glm::mat3_cast(rot) * vec3(0, 0, 1), rot, vec2{u, v});
        }
    }
    for(uint i = 0; i <= 11; ++i) {
        for(uint j = 0; j <= 24; ++j) {
            if(i < 11) {
                indices.push_back(j + i * 25);
                indices.push_back(j + i * 25 + 26);
                indices.push_back(j + i * 25 + 25);
            }
            if(i > 0) {
                indices.push_back(j + i * 25 + 0);
                indices.push_back(j + i * 25 + 1);
                indices.push_back(j + i * 25 + 26);
            }
        }
    }

    m_preview_mesh.init(std::make_unique<GeometryMesh>(vertices, indices));
    m_preview_mesh->setSubMesh("sphere", 0, indices.size());

    m_preview_model.init(std::make_unique<GeometryModel>(m_preview_mesh));
    m_preview_model->setMaterialMapping("sphere", m_materials[0].material);

    m_model_preview.assign<Geometry>(m_preview_model);

    m_material_selection->addRadioButton(u8"Dummy");
    m_material_selection->setSelection(0);
}

} // namespace bembel::tools 