#ifndef CAMERACONTROLE_H
#define CAMERACONTROLE_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include <bembel-kernel/events/event-manager.h>
#include <bembel-kernel/events/input-events.h>
#include <bembel-graphics/rendering-pipeline/camera.h>

#include <glm/glm.hpp>

#include <memory>

/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
class CameraControle
{
public:
	CameraControle( bembel::EventManager*, std::shared_ptr<bembel::Camera> camera);
	~CameraControle();

	void SetPitch(float);
	void SetYaw(float);
	void SetDist(float);

	float GetPitch() const;
	float GetYaw() const;
	float GetDist() const;

	void SetCameraOffset(const glm::vec3&);

	void Update(double dTime);

	void HandleEvent(const bembel::MouseButtonPressEvent&);
	void HandleEvent(const bembel::MouseButtonReleaseEvent&);
	void HandleEvent(const bembel::CursorMovedEvent&);
	void HandleEvent( const bembel::ScrollEvent& );

private:
	bembel::EventManager*           _eventMgr;
	std::shared_ptr<bembel::Camera> _camera;

	bool _move;

	glm::vec2 _cursorPos;
	glm::vec2 _prevPos;

	float _pitch = -0.3;
	float _yaw   = 0;
	float _dist  = 10.0f;

	glm::vec3 _offset;
};
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
