#ifndef CAMERACONTROLE_H
#define CAMERACONTROLE_H

#include <bembel/graphics/rendering-pipeline/camera.hpp>
#include <bembel/kernel/events/event-manager.hpp>
#include <bembel/kernel/events/input-events.hpp>
#include <glm/glm.hpp>
#include <memory>

namespace bembel {

class CameraControle {
  public:
    CameraControle(kernel::EventManager&, std::shared_ptr<graphics::Camera> camera);
    ~CameraControle();

    void SetPitch(float);
    void SetYaw(float);
    void SetDist(float);

    float GetPitch() const;
    float GetYaw() const;
    float GetDist() const;

    void SetCameraOffset(const glm::vec3&);

    void update(double dTime);

    void handleEvent(const kernel::MouseButtonPressEvent&);
    void handleEvent(const kernel::MouseButtonReleaseEvent&);
    void handleEvent(const kernel::CursorMovedEvent&);
    void handleEvent(const kernel::ScrollEvent&);

  private:
    kernel::EventManager&             event_mgr;
    std::shared_ptr<graphics::Camera> camera;

    bool move = false;

    glm::vec2 cursor_pos;
    glm::vec2 prev_pos;

    float pitch = -0.3;
    float yaw   = 0;
    float dist  = 10.0f;

    glm::vec3 offset;
};

} // namespace bembel
#endif // include guards
