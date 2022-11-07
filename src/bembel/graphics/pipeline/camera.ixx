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

    vec3 const& getPosition() const { return m_position; }
    void        setPosition(vec3 const& value) {
               m_position = value;
               updateViewMatrix();
    }

    quat const& getOrientation() const { return m_oriantation; }
    void        setOrientation(quat const& value) {
               m_oriantation = value;
               updateViewMatrix();
    }

    void setUpProjection(float fildOfFiew, float aspectRation, float near, float far) {
        m_proj_matrix         = glm::perspective(fildOfFiew, aspectRation, near, far);
        mat4 tmp              = m_proj_matrix;
        m_inverse_proj_matrix = glm::inverse(tmp);

        float const y         = glm::sin(fildOfFiew);
        float const x         = y * aspectRation;
        float const z         = glm::cos(fildOfFiew);
        m_view_frustum.left   = glm::normalize(vec4(-x, 0.f, z, 0.f)); //   left clipping plane
        m_view_frustum.right  = glm::normalize(vec4(+x, 0.f, z, 0.f)); //  right clipping plane
        m_view_frustum.bottom = glm::normalize(vec4(0.f, -y, z, 0.f)); // bottom clipping plane
        m_view_frustum.top    = glm::normalize(vec4(0.f, +y, z, 0.f)); //    top clipping plane
        m_view_frustum.near   = vec4(0.f, 0.f, +1.f, -near);           //   near clipping plane
        m_view_frustum.far    = vec4(0.f, 0.f, -1.f, +far);            //    far clipping plane
    }

    mat4 const& getViewMatrix() const { return m_view_matrix; }
    mat4 const& getProjectionMatrix() const { return m_proj_matrix; }

    mat4 const& getInverseViewMatrix() const { return m_inverse_view_matrix; }
    mat4 const& getInverseProjectionMatrix() const { return m_inverse_proj_matrix; }

    ViewFrustum const& getViewFrustum() const { return m_view_frustum; }

  private:
    void updateViewMatrix() {
        m_view_matrix = glm::translate(glm::mat4_cast(glm::inverse(m_oriantation)), -m_position);
        m_inverse_view_matrix = glm::affineInverse(m_view_matrix);
    }

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
