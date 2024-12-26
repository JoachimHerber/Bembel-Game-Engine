module;
#include <assimp/scene.h>

#include <assimp/Importer.hpp>
export module bembel.tools.editor:Scenes;

import std;
import bembel;

namespace bembel::tools {
using namespace bembel::base;
using namespace bembel::gui;
using namespace bembel::graphics;

struct SceneData {
    std::string            name;
    std::shared_ptr<Scene> scene;
};

export std::vector<SceneData> SCENES;

export Entity MODEL_PREVIEW_ENTITY;

export void initDefaultScenes(RenderingPipeline*);

export void drawScenePreviewUI(RenderingPipeline*);

} // namespace bembel::tools