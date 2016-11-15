/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>
/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel{

Camera::Camera()
{}

Camera::~Camera()
{}

const glm::vec3& Camera::GetPosition() const
{
	return _position;
}
void Camera::SetPosition(const glm::vec3& value)
{
	_position = value;
	_viewMatix = glm::translate(glm::mat4_cast(glm::inverse(_oriantation)), -_position);
}

const glm::quat& Camera::GetOrientation() const
{
	return _oriantation;
}
void Camera::SetOrientation(const glm::quat& value)
{
	_oriantation = value;
	_viewMatix = glm::translate(glm::mat4_cast(glm::inverse(_oriantation)), -_position);
}

void Camera::SetUpProjection(
	float fildOfFiew,
	float aspectRation,
	float near,
	float far)
{
	_projMatrix = glm::perspective(fildOfFiew, aspectRation, near, far);

	float x = sin(fildOfFiew)*aspectRation;
	float y = sin(fildOfFiew);
	float z = cos(fildOfFiew);
	_viewFrustum[1] = glm::normalize(glm::vec4(-x, 0.f, z, 0.f));//   left clipping plane
	_viewFrustum[0] = glm::normalize(glm::vec4(+x, 0.f, z, 0.f));//  right clipping plane
	_viewFrustum[2] = glm::normalize(glm::vec4(0.f, -y, z, 0.f));// bottom clipping plane
	_viewFrustum[3] = glm::normalize(glm::vec4(0.f, +y, z, 0.f));//    top clipping plane
	_viewFrustum[4] = glm::vec4(0.f, 0.f, +1.f, -near);//             near clipping plane
	_viewFrustum[5] = glm::vec4(0.f, 0.f, -1.f, +far);//               far clipping plane
}

const glm::mat4& Camera::GetViewMatrix() const
{
	return _viewMatix;
}

const glm::mat4& Camera::GetProjectionMatrix() const
{
	return _projMatrix;
}

const std::array<glm::vec4, 6>& Camera::GetViewFrustum() const
{
	return _viewFrustum;
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
