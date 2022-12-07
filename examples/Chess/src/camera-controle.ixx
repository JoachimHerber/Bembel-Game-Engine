module;
#include <memory>
export module bembel.examples.chess.camera;

import bembel;

namespace bembel::examples::chess {
using namespace base;
using namespace kernel;
using namespace graphics;

export class CameraControle {
  public:
    CameraControle(std::shared_ptr<Camera> camera);
    ~CameraControle();

    void        enableManualControle(bool b) { m_enabeld = b; }

    void        setPitch(float f) { m_pitch = f; }
    void        setYaw(float f) { m_yaw = f; }
    void        setDist(float f) { m_dist = f; }

    float       getPitch() const { return m_pitch; }
    float       getYaw() const { return m_yaw; }
    float       getDist() const { return m_dist; }

    vec3 const& getPosition() const { return m_camera->getPosition(); }

    void        setCameraOffset(vec3 const& pos) { m_offset = pos; }

    void        update(double dTime);

    void        handleEvent(MouseButtonPressEvent const&);
    void        handleEvent(MouseButtonReleaseEvent const&);
    void        handleEvent(CursorMovedEvent const&);
    void        handleEvent(ScrollEvent const&);

  private:
    std::shared_ptr<Camera> m_camera;

    bool                    m_enabeld    = false;

    bool                    m_move       = false;

    vec2                    m_cursor_pos = {0.f, 0.f};
    vec2                    m_prev_pos   = {0.f, 0.f};

    float                   m_pitch      = -0.1f;
    float                   m_yaw        = 0;
    float                   m_dist       = 10.0f;

    vec3                    m_offset     = {0.f, 0.f, 0.f};
};

} // namespace bembel
