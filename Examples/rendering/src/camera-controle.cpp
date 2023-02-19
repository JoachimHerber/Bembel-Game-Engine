module;
#include <GLFW/glfw3.h>

#include <glm/gtc/quaternion.hpp>
module bembel.examples.rendering;

namespace bembel {
using namespace bembel::base;
using namespace bembel::kernel;

CameraControle::CameraControle(std::shared_ptr<graphics::Camera> camera) : m_camera(camera) {
    events::addHandler<MouseButtonPressEvent>(this);
    events::addHandler<MouseButtonReleaseEvent>(this);
    events::addHandler<CursorMovedEvent>(this);
    events::addHandler<ScrollEvent>(this);
}

CameraControle::~CameraControle() {
    events::removeHandler<MouseButtonPressEvent>(this);
    events::removeHandler<MouseButtonReleaseEvent>(this);
    events::removeHandler<CursorMovedEvent>(this);
    events::removeHandler<ScrollEvent>(this);
}

void CameraControle::handleEvent(const MouseButtonPressEvent& event) {
    if(event.button_id == GLFW_MOUSE_BUTTON_RIGHT) m_move = true;
    if(event.button_id == GLFW_MOUSE_BUTTON_MIDDLE) m_strive = true;
}

void CameraControle::handleEvent(const MouseButtonReleaseEvent& event) {
    if(event.button_id == GLFW_MOUSE_BUTTON_RIGHT) m_move = false;
    if(event.button_id == GLFW_MOUSE_BUTTON_MIDDLE) m_strive = false;
}

void CameraControle::handleEvent(const CursorMovedEvent& event) {
    vec2 cursor_movement = m_cursor_pos - vec2(event.position);
    m_cursor_pos         = event.position;

    if(m_move) {
        m_yaw += 0.01f * cursor_movement.x;
        m_pitch += 0.01f * cursor_movement.y;
        if(m_pitch <= -1.5f) m_pitch = -1.5f;
        if(m_pitch >= +1.5f) m_pitch = +1.5f;
    }

    static constexpr vec3 X_AXIS = vec3(1, 0, 0);
    static constexpr vec3 Y_AXIS = vec3(0, 1, 0);
    static constexpr vec3 Z_AXIS = vec3(0, 0, 1);

    quat pitch       = glm::angleAxis(m_pitch, X_AXIS);
    quat yaw         = glm::angleAxis(m_yaw, Y_AXIS);
    quat orientation = yaw * pitch;
    m_camera->setOrientation(orientation);

    if(m_strive) {
        vec3 pos = m_camera->getPosition();
        mat3 rot = glm::mat3_cast(orientation);

        pos += 0.1f * cursor_movement.x * (rot * vec3(1, 0, 0));
        pos -= 0.1f * cursor_movement.y * (rot * vec3(0, 1, 0));

        m_camera->setPosition(pos);
    }
}

void CameraControle::handleEvent(const ScrollEvent& event) {
    vec3 pos = m_camera->getPosition();
    mat3 rot = glm::mat3_cast(m_camera->getOrientation());

    pos -= float(event.y) * (rot * vec3(0.f, 0.f, 2.f));

    m_camera->setPosition(pos);
}

} // namespace bembel
