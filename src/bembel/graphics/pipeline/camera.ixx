module;
export module bembel.graphics.pipeline:Camera;

import bembel.base;

namespace bembel::graphics {
using namespace bembel::base;

export struct ViewFrustum {
    vec4 left;
    vec4 right;
    vec4 bottom;
    vec4 top;
    vec4 near;
    vec4 far;
};

export class Camera final {
  public:
    Camera()  = default;
    ~Camera() = default;

    vec3 getPosition() const { return m_position; }
    void setPosition(In<vec3> value) {
        m_position = value;
        updateViewMatrix();
    }

    quat getOrientation() const { return m_oriantation; }
    void setOrientation(In<quat> value) {
        m_oriantation = value;
        updateViewMatrix();
    }

    void setUpProjection(float fildOfFiew, float aspectRation, float near, float far);

    mat4 const& getViewMatrix() const { return m_view_matrix; }
    mat4 const& getProjectionMatrix() const { return m_proj_matrix; }

    mat4 const& getInverseViewMatrix() const { return m_inverse_view_matrix; }
    mat4 const& getInverseProjectionMatrix() const { return m_inverse_proj_matrix; }

    ViewFrustum const& getViewFrustum() const { return m_view_frustum; }

  private:
    void updateViewMatrix();

    vec3 m_position    = {0.f, 0.f, 0.f};
    quat m_oriantation = {0.f, 0.f, 0.f, 1.f};

    mat4 m_view_matrix;
    mat4 m_proj_matrix;

    mat4 m_inverse_view_matrix;
    mat4 m_inverse_proj_matrix;

    // clipping planes in order: left, right, bottom, top, near, far
    ViewFrustum m_view_frustum;
};

} // namespace bembel::graphics
