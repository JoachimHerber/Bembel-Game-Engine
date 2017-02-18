#ifndef CAMERACONTROLE_H
#define CAMERACONTROLE_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include <BembelKernel/Events/EventManager.h>
#include <BembelKernel/Events/InputEvents.h>
#include <BembelGraphics/RenderingPipeline/Camera.h>

#include <glm/glm.hpp>

#include <memory>

/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel{

class CameraControle
{
public:
	using CameraPtr   = std::shared_ptr<bembel::Camera>;

	CameraControle(EventManager*, CameraPtr camera);
	~CameraControle();

	void SetPitch(float);
	void SetYaw(float);
	void SetDist(float);

	float GetPitch() const;
	float GetYaw() const;
	float GetDist() const;

	void EnableManualControle(bool b);

	void SetCameraOffset(const glm::vec3&);

	void Update(double dTime);

	void HandleEvent(const bembel::MouseButtonPressEvent&);
	void HandleEvent(const bembel::MouseButtonReleaseEvent&);
	void HandleEvent(const bembel::CursorMovedEvent&);

private:
	EventManager* _eventMgr;
	CameraPtr     _camera;

	bool _enabeld = false;

	bool _move;
	bool _zoom;

	glm::vec2 _cursorPos;
	glm::vec2 _prevPos;

	float _pitch = 0;
	float _yaw   = 0;
	float _dist  = 1.0f;

	glm::vec3 _offset;
};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
