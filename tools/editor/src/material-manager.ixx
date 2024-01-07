module;
#include <assimp/scene.h>

#include <assimp/Importer.hpp>
#include <memory>
#include <string_view>
export module bembel.tools.editor:MaterialMgr;

import bembel;

namespace bembel::tools {
using namespace bembel::base;
using namespace bembel::gui;
using namespace bembel::graphics;

export class MaterialManager {
  public:
    MaterialManager(In<TabGroupWidget::Tab*> tab, In<RenderingPipeline*> pipeline);
    ~MaterialManager() = default;

    void parseScene(aiScene const* ai_scene);

    Asset<Material> getMaterial(aiScene const* ai_scene, uint index){
        for(auto& mat : m_materials) {
            if(mat.scene == ai_scene && mat.index == index) return mat.material;
        }
        return m_materials[0].material;
    }

  private:
    void onTabSelected();

    void onMaterialSelection(int index);

    void onUpdateMaterialParam(In<i64>);

    void onTextUpdate(In<std::u8string>, In < std::u8string>);

    void loadEmissionTexture();
    void loadBaseColorTexture();
    void loadMaterialPropsTexture();
    void loadNormalMapTexture();

    void initPreview();

  private:
    RenderingPipeline*     m_pipeline;
    std::shared_ptr<Scene> m_scene;

    Entity m_model_preview;

    TabGroupWidget::Tab* m_tab;

    RadioButtonGroupWidget* m_material_selection;

    IntSliderWidget*   m_slider_widgets[9];
    TextInputWidget*   m_texture_path_inputs[4];
    ButtonWidget*      m_load_texture_button[4];

    struct Mat {
        Mat(aiScene const* scene, uint idx);

        aiScene const* scene;
        uint           index;

        Asset<Material> material;

        std::u8string name = u8"";

        std::filesystem::path emission_texture       = "";
        std::filesystem::path base_color_texture     = "";
        std::filesystem::path material_props_texture = "";
        std::filesystem::path normal_map_texture     = "";
    };

    std::vector<Mat> m_materials;

    Asset<GeometryMesh>  m_preview_mesh;
    Asset<GeometryModel> m_preview_model;
};

} // namespace bembel::tools