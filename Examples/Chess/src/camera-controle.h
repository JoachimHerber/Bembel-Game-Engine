#ifndef CAMERACONTROLE_H
#define CAMERACONTROLE_H

#include <bembel/bembel.hpp>
#include <glm/glm.hpp>
#include <memory>

class CameraControle {
  public:
    CameraControle(bembel::kernel::EventManager&, std::shared_ptr<bembel::graphics::Camera> camera);
    ~CameraControle();

    void setPitch(float);
    void setYaw(float);
    void setDist(float);

    float getPitch() const;
    float getYaw() const;
    float getDist() const;

    void enableManualControle(bool b);

    void setCameraOffset(const glm::vec3&);

    void update(double dTime);

    void handleEvent(const bembel::kernel::MouseButtonPressEvent&);
    void handleEvent(const bembel::kernel::MouseButtonReleaseEvent&);
    void handleEvent(const bembel::kernel::CursorMovedEvent&);
    void handleEvent(const bembel::kernel::ScrollEvent&);

  private:
    bembel::kernel::EventManager&             event_mgr;
    std::shared_ptr<bembel::graphics::Camera> camera;

    bool enabeld = false;

    bool move;

    glm::vec2 cursor_pos;
    glm::vec2 prev_pos;

    float pitch = -0.1f;
    float yaw   = 0;
    float dist  = 10.0f;

    glm::vec3 offset;
};
#endif // include guards
