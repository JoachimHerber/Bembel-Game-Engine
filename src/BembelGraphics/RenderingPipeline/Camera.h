#ifndef BEMBEL_CAMERA_H
#define BEMBEL_CAMERA_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include <BembelConfig.h>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <array>

/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel {

class BEMBEL_API Camera final
{
public:
	Camera();
	~Camera();

	const glm::vec3& GetPosition() const;
	void SetPosition(const glm::vec3&);

	const glm::quat& GetOrientation() const;
	void SetOrientation(const glm::quat&);

	void SetUpProjection(float fildOfFiew, float aspectRation, float near, float far);

	const glm::mat4& GetViewMatrix() const;
	const glm::mat4& GetProjectionMatrix() const;

	const std::array<glm::vec4, 6>& GetViewFrustum() const;

private:
	glm::vec3 _position;
	glm::quat _oriantation;

	glm::mat4 _projMatrix;
	glm::mat4 _viewMatix;

	// clipping planes in order: left, right, bottom, top, near, far
	std::array<glm::vec4, 6> _viewFrustum;
};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
