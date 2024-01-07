module;
#include <assimp/scene.h>

#include <assimp/Importer.hpp>
#include <memory>
#include <string_view>
export module bembel.tools.editor:ModelMgr;

import bembel;
import :MaterialMgr;

namespace bembel::tools {
using namespace bembel::base;
using namespace bembel::gui;
using namespace bembel::graphics;

export class ModelManager {
  public:
    ModelManager(
        In<TabGroupWidget::Tab*> tab,
        In<RenderingPipeline*>   pipeline,
        In<MaterialManager*>     materials
    );
    ~ModelManager() = default;

    void parseScene(aiScene const* ai_scene);

  private:
    void onTabSelected();

    void onModelSelection(int index);

  private:
    TabGroupWidget::Tab*   m_tab;
    RenderingPipeline*     m_pipeline;
    MaterialManager*       m_materials;

    std::shared_ptr<Scene> m_scene;

    RadioButtonGroupWidget* m_model_selection;

    Entity m_model_preview;

    std::vector<Asset<GeometryMesh>>  m_meshes;
    std::vector<Asset<GeometryModel>> m_models;
};

} // namespace bembel::tools