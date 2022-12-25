module;
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
module bembel.examples.chess.camera;

namespace bembel::examples::chess {
using namespace bembel::base;
using namespace bembel::kernel;

CameraControle::CameraControle(std::shared_ptr<graphics::Camera> camera) : m_camera(camera) {
    events::addHandler<kernel::MouseButtonPressEvent>(this);
    events::addHandler<kernel::MouseButtonReleaseEvent>(this);
    events::addHandler<kernel::CursorMovedEvent>(this);
    events::addHandler<kernel::ScrollEvent>(this);
}

CameraControle::~CameraControle() {
    events::removeHandler<kernel::MouseButtonPressEvent>(this);
    events::removeHandler<kernel::MouseButtonReleaseEvent>(this);
    events::removeHandler<kernel::CursorMovedEvent>(this);
    events::removeHandler<kernel::ScrollEvent>(this);
}

void CameraControle::update(double dTime) {
    vec2 cursor_movement = m_cursor_pos - m_prev_pos;
    m_prev_pos           = m_cursor_pos;

    if(m_enabeld && m_move) {
        m_yaw -= 0.01f * cursor_movement.x;
        m_pitch -= 0.01f * cursor_movement.y;
        if(m_pitch <= -1.5f) m_pitch = -1.5f;
        if(m_pitch >= +1.5f) m_pitch = +1.5f;
    }

    static constexpr vec3 X_AXIS = vec3(1, 0, 0);
    static constexpr vec3 Y_AXIS = vec3(0, 1, 0);
    static constexpr vec3 Z_AXIS = vec3(0, 0, 1);

    quat pitch = glm::angleAxis(m_pitch, X_AXIS);
    quat yaw   = glm::angleAxis(m_yaw, Y_AXIS);
    m_camera->setOrientation(yaw * pitch);

    vec3 pos = m_offset + m_dist * glm::mat3_cast(yaw * pitch) * Z_AXIS;
    m_camera->setPosition(pos);
}

void CameraControle::handleEvent(const kernel::MouseButtonPressEvent& event) {
    if(event.button_id == 0) m_move = true;
}

void CameraControle::handleEvent(const kernel::MouseButtonReleaseEvent& event) {
    if(event.button_id == 0) m_move = false;
}

void CameraControle::handleEvent(const kernel::CursorMovedEvent& event) {
    m_cursor_pos = event.position;
}

void CameraControle::handleEvent(const kernel::ScrollEvent& event) {
    m_dist += event.y;
    if(m_dist < 1) m_dist = 1.0f;
}

} // namespace bembel::examples::chess
