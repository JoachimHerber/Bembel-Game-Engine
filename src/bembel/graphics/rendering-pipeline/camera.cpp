#include "./camera.hpp"

namespace bembel::graphics {

Camera::Camera() {
}

Camera::~Camera() {
}

const glm::vec3& Camera::getPosition() const {
    return this->position;
}
void Camera::setPosition(const glm::vec3& value) {
    this->position = value;
    this->view_matrix =
        glm::translate(glm::mat4_cast(glm::inverse(this->oriantation)), -this->position);
    this->inverse_view_matrix = glm::affineInverse(this->view_matrix);
}

const glm::quat& Camera::getOrientation() const {
    return this->oriantation;
}
void Camera::setOrientation(const glm::quat& value) {
    this->oriantation = value;
    this->view_matrix =
        glm::translate(glm::mat4_cast(glm::inverse(this->oriantation)), -this->position);
    this->inverse_view_matrix = glm::affineInverse(this->view_matrix);
}

void Camera::setUpProjection(float fildOfFiew, float aspectRation, float near, float far) {
    this->proj_matrix         = glm::perspective(fildOfFiew, aspectRation, near, far);
    glm::mat4 tmp             = this->proj_matrix;
    this->inverse_proj_matrix = glm::inverse(tmp);

    float x               = sin(fildOfFiew) * aspectRation;
    float y               = sin(fildOfFiew);
    float z               = cos(fildOfFiew);
    this->view_frustum[1] = glm::normalize(glm::vec4(-x, 0.f, z, 0.f)); //   left clipping plane
    this->view_frustum[0] = glm::normalize(glm::vec4(+x, 0.f, z, 0.f)); //  right clipping plane
    this->view_frustum[2] = glm::normalize(glm::vec4(0.f, -y, z, 0.f)); // bottom clipping plane
    this->view_frustum[3] = glm::normalize(glm::vec4(0.f, +y, z, 0.f)); //    top clipping plane
    this->view_frustum[4] = glm::vec4(0.f, 0.f, +1.f, -near); //             near clipping plane
    this->view_frustum[5] = glm::vec4(0.f, 0.f, -1.f, +far);  //               far clipping plane
}

const glm::mat4& Camera::getViewMatrix() const {
    return this->view_matrix;
}

const glm::mat4& Camera::getProjectionMatrix() const {
    return this->proj_matrix;
}

const glm::mat4& Camera::getInverseViewMatrix() const {
    return this->inverse_view_matrix;
}

const glm::mat4& Camera::getInverseProjectionMatrix() const {
    return this->inverse_proj_matrix;
}

const std::array<glm::vec4, 6>& Camera::getViewFrustum() const {
    return this->view_frustum;
}

} // namespace bembel::graphics
