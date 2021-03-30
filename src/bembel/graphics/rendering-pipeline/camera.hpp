#ifndef BEMBEL_GRAPHICS_RENDERING_PIPELINE_CAMERA_HPP
#define BEMBEL_GRAPHICS_RENDERING_PIPELINE_CAMERA_HPP

#include <bembel/library.hpp>

namespace bembel::graphics {

class BEMBEL_API Camera final {
  public:
    Camera();
    ~Camera();

    const glm::vec3& getPosition() const;
    void setPosition(const glm::vec3&);

    const glm::quat& getOrientation() const;
    void setOrientation(const glm::quat&);

    void setUpProjection(float fildOfFiew, float aspectRation, float near, float far);

    const glm::mat4& getViewMatrix() const;
    const glm::mat4& getProjectionMatrix() const;

    const glm::mat4& getInverseViewMatrix() const;
    const glm::mat4& getInverseProjectionMatrix() const;

    const std::array<glm::vec4, 6>& getViewFrustum() const;

  private:
    glm::vec3 position;
    glm::quat oriantation;

    glm::mat4 view_matrix;
    glm::mat4 proj_matrix;

    glm::mat4 inverse_view_matrix;
    glm::mat4 inverse_proj_matrix;

    // clipping planes in order: left, right, bottom, top, near, far
    std::array<glm::vec4, 6> view_frustum;
};

} // namespace bembel::graphics
#endif // include guards
