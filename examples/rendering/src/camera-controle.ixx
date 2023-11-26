module;
#include <memory>
export module bembel.examples.rendering : CameraControle;

import bembel;

namespace bembel {
using namespace base;
using namespace kernel;
using namespace graphics;

class CameraControle {
  public:
    CameraControle(std::shared_ptr<Camera> camera);
    ~CameraControle();

    void setPitch(float f) { m_pitch = f; }
    void setYaw(float f) { m_yaw = f; }

    float getPitch() const { return m_pitch; }
    float getYaw() const { return m_yaw; }

    vec3 getPosition() const { return m_camera->getPosition(); }

    void handleEvent(MouseButtonPressEvent const&);
    void handleEvent(MouseButtonReleaseEvent const&);
    void handleEvent(CursorMovedEvent const&);
    void handleEvent(ScrollEvent const&);

  private:
    std::shared_ptr<Camera> m_camera;

    bool m_move   = false;
    bool m_strive = false;

    vec2 m_cursor_pos = {0.f, 0.f};

    float m_pitch = -0.3f;
    float m_yaw   = 0;
};

} // namespace bembel
