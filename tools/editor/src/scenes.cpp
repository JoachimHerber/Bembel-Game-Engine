module;
#include <assimp/postprocess.h> // Post processing flags
#include <assimp/scene.h>       // Output data structure

#include <assimp/Importer.hpp> // C++ importer interface
#include <format>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <memory>
#include <numbers>
#include <optional>
#include <string_view>
module bembel.tools.editor;

import bembel;
import :Scenes;
import :Materials;

namespace bembel::tools {
using namespace bembel::base;
using namespace bembel::gui;
using namespace bembel::graphics;

Scene* createScene(In<std::string_view> name) {
    SCENES.emplace_back(std::string(name), std::make_shared<Scene>());
    auto scene = SCENES.back().scene.get();
    scene->registerComponentTypes<Transform, Geometry, DirectionalLight>();

    Entity ligth0 = {*scene, scene->createEntity()};
    Entity ligth1 = {*scene, scene->createEntity()};
    Entity ligth2 = {*scene, scene->createEntity()};

    ligth0.assign<DirectionalLight>(vec3(0.6f), glm::normalize(glm::vec3(-0.3, -1, -0.2)), true);
    ligth1.assign<DirectionalLight>(vec3(0.3f), glm::normalize(glm::vec3(0.7, -1, 0.0)), true);
    ligth2.assign<DirectionalLight>(vec3(0.2f), glm::normalize(glm::vec3(0.3, -1, +0.6)), true);
    return scene;
}

void initDefaultScenes(RenderingPipeline* pipeline) {
    if(!SCENES.empty()) return;

    auto* mat_preview =createScene("Material Preview");
    auto* model_preview = createScene("Model Preview");

    Entity material_preview = {*mat_preview, mat_preview->createEntity()};
    material_preview.assign<Transform>(vec3(0.f, 0.f, 0.f));

    MODEL_PREVIEW_ENTITY = Entity{*model_preview, model_preview->createEntity()};
    MODEL_PREVIEW_ENTITY.assign<Transform>(vec3(0.f, 0.f, 0.f));

    material_preview.assign<Geometry>(initMaterialPreviewModel());

    pipeline->setScene(SCENES[0].scene);
}

class CameraControl {
  public:
    ~CameraControl() { disable(); }
    void enable() {
        if(m_enabled == true) return;
        m_enabled = true;
        events::addHandler<kernel::MouseButtonPressEvent>(this);
        events::addHandler<kernel::KeyReleaseEvent>(this);
        events::addHandler<kernel::CursorMovedEvent>(this);
        events::addHandler<kernel::ScrollEvent>(this);

        events::broadcast(SetCursorModeEvent(CursorMode::DISABLED, WindowId(0)));
        //@ToDo lock cursor position
    }

    void disable() {
        if(m_enabled == false) return;
        m_enabled = false;
        events::removeHandler<kernel::MouseButtonPressEvent>(this);
        events::removeHandler<kernel::KeyReleaseEvent>(this);
        events::removeHandler<kernel::CursorMovedEvent>(this);
        events::removeHandler<kernel::ScrollEvent>(this);
        m_cursor_pos = {};
        events::broadcast(SetCursorModeEvent(CursorMode::NORMAL, WindowId(0)));
    }

    void updateCamera(InOut<Camera> cam) {
        quat pitch = glm::angleAxis(m_pitch, vec3(1, 0, 0));
        quat yaw   = glm::angleAxis(m_yaw, vec3(0, 1, 0));
        cam.setOrientation(yaw * pitch);

        vec3 pos = m_dist * glm::mat3_cast(yaw * pitch) * vec3(0, 0, 1);
        cam.setPosition(pos);
    }

    void handleEvent(In<MouseButtonPressEvent> event) {
        if(event.button_id == 1) disable();
    }
    void handleEvent(In<KeyReleaseEvent> event) {
        if(event.key_id == Keyboard::KeyId::ESCAPE) disable();
    }
    void handleEvent(In<CursorMovedEvent> event) {
        if(m_cursor_pos) {
            vec2 cursor_movement = vec2(event.position) - *m_cursor_pos;
            m_yaw -= 0.01f * cursor_movement.x;
            m_pitch -= 0.01f * cursor_movement.y;
            if(m_pitch <= -1.5f) m_pitch = -1.5f;
            if(m_pitch >= +1.5f) m_pitch = +1.5f;
        }
        m_cursor_pos = event.position;
    }
    void handleEvent(In<ScrollEvent> event) {
        m_dist += event.y;
        if(m_dist < 1) m_dist = 1.0f;
    }

  private:
    bool m_enabled = false;

    std::optional<vec2> m_cursor_pos;

    float m_pitch = 0.0f;
    float m_yaw   = 0.0f;
    float m_dist  = 2.5f;
};

void drawScenePreviewUI(RenderingPipeline* pipeline) {
    float const width   = imgui::GetContentRegionAvail().x;
    float const height  = imgui::GetContentRegionAvail().y;
    auto const  origin  = imgui::GetCursorScreenPos();
    auto const  cam     = pipeline->getCamera();
    auto const  texture = pipeline->getTexture("light");

    static CameraControl cam_ctrl;

    if(texture) {
        imgui::SetCursorScreenPos(origin);
        imgui::SetNextItemAllowOverlap();
        imgui::Image(
            (void*)(intptr_t)texture->getTextureHandle(),
            imgui::ImVec2(width, height),
            imgui::ImVec2(0.0f, 1.0f),
            imgui::ImVec2(1.0f, 0.0f)
        );
    }
    imgui::SetCursorScreenPos(origin);
    imgui::SetNextItemAllowOverlap();

    static uint selected_scene = 0;
    if(imgui::BeginCombo("Scene", SCENES[selected_scene].name.c_str(), 0)) {
        for(auto& it : SCENES) {
            if(imgui::Selectable(it.name.c_str(), selected_scene == 0)) {
                selected_scene = 0;
                pipeline->setScene(it.scene);
            };
        }
        imgui::EndCombo();
    }
    auto const cursor = imgui::GetCursorScreenPos();
    if(imgui::InvisibleButton(
           "EnableCameraControl", ImVec2{width, height - cursor.y}
       )) // clicked viewport
    {
        cam_ctrl.enable();
    }

    cam_ctrl.updateCamera(*cam);
    cam->setUpProjection(glm::radians(60.0f), width / height, 0.1f, 100.0f);
}

} // namespace bembel::tools