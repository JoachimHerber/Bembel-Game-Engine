module;
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
module bembel.examples.gm_helper;

import std;
import :Components;
import :Selection;
import :RenderingStages;
import :UI;
import :Texture;
import :Maps;

namespace bembel::examples::gm_helper {
using namespace bembel::base;

Application::Application() {
    using Stage = RenderingPipeline::Stage;
    Stage::registerStageType<SpriteRenderingStage>("SpriteRenderingStage");
    Stage::registerStageType<TokenRenderingStage>("TokenRenderingStage");
    Stage::registerStageType<CreateLightingOverlayStage>("CreateLightingOverlayStage");
    Stage::registerStageType<ApplyLightingOverlayStage>("ApplyLightingOverlayStage");
    Stage::registerStageType<InitiativeOverlayStage>("InitiativeOverlayStage");
}

Application::~Application() {}

std::optional<std::filesystem::path> findExisting(std::initializer_list<std::filesystem::path> paths
) {
    for(auto path : paths)
        if(std::filesystem::exists(path)) return path;
    return {};
}

bool Application::init(std::span<std::string_view> args) {
    if(auto path = findExisting({"gm-helper/config.xml", "config.xml"})) {
        if(!m_engine.loadSetting(*path)) {
            logError("Couldn't find config.xml");
            return false;
        }
    } else {
        logError("Couldn't load config.xml");
        return false;
    }

    kernel::i18n::Localisation::init(args, "local");

    if(auto path = findExisting(
           {"../assets/textures/tokens", "./assets/textures/tokens", "./textures/tokens", "./tokens"
           }
       )) {
        loadTokenTextrues(*path);
    } else {
        logError("Couldn't find token textures");
        return false;
    }

    m_primary_window   = m_engine.display.getWindow(0);
    m_secondary_window = m_engine.display.getWindow(1);
    if(!m_primary_window || !m_secondary_window) {
        logError("No primary or secondary window");
        return false;
    }

    auto* viewport = m_primary_window->getViewport(0);
    if(!viewport) {
        logError("No Viewport");
        return false;
    }
    viewport->addView(this);

    auto* primary_pipeline   = getSystem<GraphicSystem>()->getRenderingPipelines()[0].get();
    auto* secondary_pipeline = getSystem<GraphicSystem>()->getRenderingPipelines()[1].get();

    if(auto path = findExisting({"./gm-helper/maps", "./maps"})) {
        logInfo("loading maps: '{}'", path->string());
        if(!loadMaps(*path)) {
            logError("Couldn't load maps");
            return false;
        }
    } else {
        logError("Couldn't find maps");
        return false;
    }

    primary_pipeline->setScene(getCurrentScene());
    secondary_pipeline->setScene(getCurrentScene());

    primary_pipeline->setCamera(m_primary_camera);
    secondary_pipeline->setCamera(m_secondary_camera);

    float ar = float(m_primary_window->getWindowSize().x) //
             / float(m_primary_window->getWindowSize().y);

    m_primary_camera->setPosition(vec3(0, 0, 0));
    m_primary_camera->setOrientation(quat(1, 0, 0, 0));
    m_primary_camera->setOrtho(
        -ar * m_primary_zoom, ar * m_primary_zoom, -m_primary_zoom, m_primary_zoom, -1, 1
    );

    m_secondary_camera->setPosition(vec3(0, 0, 0));
    m_secondary_camera->setOrientation(quat(1, 0, 0, 0));
    m_secondary_camera->setOrtho(
        -ar * m_secondary_zoom, ar * m_secondary_zoom, -m_secondary_zoom, m_secondary_zoom, -1, 1
    );

    m_engine.initSystems();
    return true;
}

void Application::cleanup() {
    g_token_textures.clear();
    g_maps.clear();

    m_engine.shutdownSystems();
    m_engine.display.closeOpenWindows();
}

void Application::update(double time) {}

void Application::handleEvent(In<WindowShouldCloseEvent> event) {
    quit();
}

void Application::handleEvent(In<KeyReleaseEvent> event) {
    if(event.window->getWindowID() != WindowId::MAIN) return;
    auto& io = imgui::GetIO();
    if(io.WantCaptureMouse || io.WantCaptureKeyboard) return;

    auto setMonitor = [this](int i) {
        int           monitor_count;
        GLFWmonitor** monitors = glfwGetMonitors(&monitor_count);
        if(i >= monitor_count) return;

        int xpos, ypos, width, height;
        glfwGetMonitorWorkarea(monitors[i], &xpos, &ypos, &width, &height);

        m_secondary_window->setWindowPosition({xpos, ypos});
        m_secondary_window->setWindowSize({width, height});

        float ar = float(width) / float(height);

        m_secondary_camera->setOrtho(
            -ar * m_secondary_zoom,
            ar * m_secondary_zoom,
            -m_secondary_zoom,
            m_secondary_zoom,
            -1,
            1
        );
    };

    auto updateZoom = [this](float change) {
        m_secondary_zoom = std::clamp(m_secondary_zoom + change, 5.f, 50.f);

        float ar = float(m_secondary_window->getWindowSize().x)
                 / float(m_secondary_window->getWindowSize().y);

        m_secondary_camera->setOrtho(
            -ar * m_secondary_zoom,
            ar * m_secondary_zoom,
            -m_secondary_zoom,
            m_secondary_zoom,
            -1,
            1
        );
    };

    auto moveSecondaryCamera = [this](float dx, float dy) {
        vec3 cam_pos = m_secondary_camera->getPosition();

        cam_pos.x = std::clamp(cam_pos.x + dx, -25.f, 25.f);
        cam_pos.y = std::clamp(cam_pos.y + dy, -25.f, 25.f);

        m_secondary_camera->setPosition(cam_pos);
    };

    switch(event.key_id) {
        case Keyboard::KeyId::F1: setMonitor(0); break;
        case Keyboard::KeyId::F2: setMonitor(1); break;
        case Keyboard::KeyId::F3: setMonitor(2); break;
        case Keyboard::KeyId::F4: setMonitor(3); break;
        case Keyboard::KeyId::PAGE_UP: updateZoom(+1.f); break;
        case Keyboard::KeyId::PAGE_DOWN: updateZoom(-1.f); break;
        case Keyboard::KeyId::LEFT: moveSecondaryCamera(-1.f, +0.f); break;
        case Keyboard::KeyId::RIGHT: moveSecondaryCamera(+1.f, +0.f); break;
        case Keyboard::KeyId::DOWN: moveSecondaryCamera(+0.f, -1.f); break;
        case Keyboard::KeyId::UP: moveSecondaryCamera(+0.f, +1.f); break;
        default: break;
    }
}

void Application::handleEvent(In<MouseButtonPressEvent> event) {
    if(event.window->getWindowID() != WindowId::MAIN) return;
    auto& io = imgui::GetIO();
    if(io.WantCaptureMouse || io.WantCaptureKeyboard) return;

    if(event.button_id == 0) m_dragging_active = true;
}

void Application::handleEvent(In<MouseButtonReleaseEvent> event) {
    if(event.window->getWindowID() != WindowId::MAIN) return;
    auto& io = imgui::GetIO();
    if(io.WantCaptureMouse || io.WantCaptureKeyboard) return;

    switch(event.button_id) {
        case 0: m_dragging_active = false; break;
        case 1: openContextMenu(m_cursor_pos); break;
        default: break;
    }
}
void Application::handleEvent(In<CursorMovedEvent> event) {
    if(event.window->getWindowID() != WindowId::MAIN) return;
    auto& io = imgui::GetIO();
    if(io.WantCaptureMouse || io.WantCaptureKeyboard) return;

    vec4 pos = {event.position.x, event.position.y, 0.f, 1};
    pos.x /= event.window->getWindowSize().x;
    pos.y /= event.window->getWindowSize().y;
    pos.x = (2.f * pos.x - 1.f);
    pos.y = -(2.f * pos.y - 1.f);
    pos   = m_primary_camera->getInverseProjectionMatrix() * pos;
    pos /= pos.w;
    pos += vec4(m_primary_camera->getPosition(), 0.f);

    if(m_dragging_active) {
        if(auto selected_entity = getSelection()) {
            m_cursor_pos                                = pos;
            selected_entity->get<Transform>()->position = vec3(m_cursor_pos, 0.0f);
            if(auto token = selected_entity->get<Token>()) {
                if(token->is_player) m_secondary_camera->setPosition(vec3(m_cursor_pos, 0.0f));
            }
        } else {
            vec2 cursor_movement = vec2(pos) - m_cursor_pos;

            vec3 cam_pos = m_primary_camera->getPosition();

            cam_pos.x -= cursor_movement.x;
            cam_pos.y -= cursor_movement.y;
            cam_pos.x = std::clamp(cam_pos.x, -25.f, 25.f);
            cam_pos.y = std::clamp(cam_pos.y, -25.f, 25.f);

            m_primary_camera->setPosition(cam_pos);
        }
    } else {
        m_cursor_pos = pos;
        updateSelection(m_cursor_pos);
    }
}

void Application::handleEvent(In<ScrollEvent> event) {
    if(event.window->getWindowID() != WindowId::MAIN) return;
    auto& io = imgui::GetIO();
    if(io.WantCaptureMouse || io.WantCaptureKeyboard) return;

    if(auto selected_entity = getSelection()) {
        if(m_engine.input.keyboard.getKey(Keyboard::KeyId::LEFT_CONTROL)->getIsPressed()) {
            if(auto transform = selected_entity->get<Transform>()) {
                transform->rotation *=
                    glm::angleAxis(glm::radians(15.f * float(event.y)), vec3(0.0f, 0.0f, 1.0f));
                return;
            }
        } else {
            if(auto area_marker = selected_entity->get<AreaMarker>()) {
                area_marker->size = std::clamp(area_marker->size - float(event.y), 1.f, 20.f);
                if(auto selectable = selected_entity->get<Selectable>())
                    selectable->dist = 0.5 * area_marker->size;
                return;
            }
        }
    }
    m_primary_zoom -= event.y;
    m_primary_zoom = std::clamp(m_primary_zoom, 1.f, 25.f);
    float ar = float(event.window->getWindowSize().x) / float(event.window->getWindowSize().y);
    m_primary_camera->setOrtho(
        -ar * m_primary_zoom, ar * m_primary_zoom, -m_primary_zoom, m_primary_zoom, -1, 1
    );
}

void Application::drawUI(In<ivec2> view_port_pos, In<uvec2> view_port_size) {
    auto viewport = imgui::GetMainViewport();

    imgui::SetNextWindowPos(imgui::ImVec2(view_port_pos.x, view_port_pos.y));
    imgui::SetNextWindowSize(imgui::ImVec2(view_port_size.x, view_port_size.y));
    imgui::SetNextWindowViewport(viewport->ID);
    imgui::PushStyleVar(imgui::ImGuiStyleVar_WindowRounding, 0.0f);
    imgui::PushStyleVar(imgui::ImGuiStyleVar_WindowBorderSize, 0.0f);
    imgui::PushStyleVar(imgui::ImGuiStyleVar_WindowPadding, imgui::ImVec2(0.0f, 0.0f));

    int dockspace_flags =
        imgui::ImGuiDockNodeFlags_None | imgui::ImGuiDockNodeFlags_PassthruCentralNode;
    int window_flags = imgui::ImGuiWindowFlags_MenuBar | imgui::ImGuiWindowFlags_NoDocking
                     | imgui::ImGuiWindowFlags_NoTitleBar | imgui::ImGuiWindowFlags_NoCollapse
                     | imgui::ImGuiWindowFlags_NoResize | imgui::ImGuiWindowFlags_NoMove
                     | imgui::ImGuiWindowFlags_NoBackground
                     | imgui::ImGuiWindowFlags_NoBringToFrontOnFocus
                     | imgui::ImGuiWindowFlags_NoNavFocus;

    if(imgui::Begin("DockSpace", nullptr, window_flags)) {
        drawMainMenu(getSystem<GraphicSystem>());
        imgui::DockSpace(imgui::GetID("MyDockSpace"), {0.0f, 0.0f}, dockspace_flags);
    }
    imgui::End();
    imgui::PopStyleVar(3);

    gm_helper::drawUI(m_primary_camera.get(), m_secondary_camera.get());
}

} // namespace bembel::examples::gm_helper
