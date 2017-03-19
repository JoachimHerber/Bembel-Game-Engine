/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel {

Camera::Camera()
{}

Camera::~Camera()
{}

const glm::vec3& Camera::GetPosition() const
{
	return position_;
}
void Camera::SetPosition(const glm::vec3& value)
{
	position_ = value;
	view_matrix_ = glm::translate(
		glm::mat4_cast(glm::inverse(oriantation_)), -position_);
	inverse_view_matrix_ = glm::affineInverse(view_matrix_);
}

const glm::quat& Camera::GetOrientation() const
{
	return oriantation_;
}
void Camera::SetOrientation(const glm::quat& value)
{
	oriantation_ = value;
	view_matrix_ = glm::translate(
		glm::mat4_cast(glm::inverse(oriantation_)), -position_);
	inverse_view_matrix_ = glm::affineInverse(view_matrix_);
}

void Camera::SetUpProjection(
	float fildOfFiew,
	float aspectRation,
	float near,
	float far)
{
	proj_matrix_ = glm::perspective(fildOfFiew, aspectRation, near, far);
	glm::mat4 tmp = proj_matrix_;
	inverse_proj_matrix_ = glm::inverse(tmp);

	float x = sin(fildOfFiew)*aspectRation;
	float y = sin(fildOfFiew);
	float z = cos(fildOfFiew);
	view_frustum_[1] = glm::normalize(glm::vec4(-x, 0.f, z, 0.f));//   left clipping plane
	view_frustum_[0] = glm::normalize(glm::vec4(+x, 0.f, z, 0.f));//  right clipping plane
	view_frustum_[2] = glm::normalize(glm::vec4(0.f, -y, z, 0.f));// bottom clipping plane
	view_frustum_[3] = glm::normalize(glm::vec4(0.f, +y, z, 0.f));//    top clipping plane
	view_frustum_[4] = glm::vec4(0.f, 0.f, +1.f, -near);//             near clipping plane
	view_frustum_[5] = glm::vec4(0.f, 0.f, -1.f, +far);//               far clipping plane
}

const glm::mat4& Camera::GetViewMatrix() const
{
	return view_matrix_;
}

const glm::mat4& Camera::GetProjectionMatrix() const
{
	return proj_matrix_;
}

const glm::mat4& Camera::GetinverseViewMatrix() const
{
	return inverse_view_matrix_;
}

const glm::mat4& Camera::GetinverseProjectionMatrix() const
{
	return inverse_proj_matrix_;
}

const std::array<glm::vec4, 6>& Camera::GetViewFrustum() const
{
	return view_frustum_;
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
