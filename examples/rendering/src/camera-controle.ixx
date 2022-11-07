﻿module;
#include <memory>
export module bembel.examples.rendering : CameraControle;

import bembel;

namespace bembel {
using namespace base;
using namespace kernel;
using namespace graphics;

class CameraControle {
  public:
    CameraControle(EventManager&, std::shared_ptr<Camera> camera);
    ~CameraControle();

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
    EventManager&           m_event_mgr;
    std::shared_ptr<Camera> m_camera;


    bool                    m_move       = false;

    vec2                    m_cursor_pos = {0.f, 0.f};
    vec2                    m_prev_pos   = {0.f, 0.f};

    float                   m_pitch      = -0.3f;
    float                   m_yaw        = 0;
    float                   m_dist       = 10.0f;

    vec3                    m_offset     = {0.f, 0.f, 0.f};
};

} // namespace bembel
