#ifndef CAMERACONTROLE_H
#define CAMERACONTROLE_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include <BembelBase/Events/EventManager.h>
#include <BembelKernel/Events/InputEvents.h>
#include <BembelGraphics/RenderingPipeline/Camera.h>

#include <glm/glm.hpp>

#include <memory>

/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
class CameraControle
{
public:
	using EventMgrPtr = std::shared_ptr<bembel::EventManager>;
	using CameraPtr   = std::shared_ptr<bembel::Camera>;

	CameraControle(EventMgrPtr, CameraPtr camera);
	~CameraControle();

	void Update(double dTime);

	void HandleEvent(const bembel::MouseButtonPressEvent&);
	void HandleEvent(const bembel::MouseButtonReleaseEvent&);
	void HandleEvent(const bembel::CursorMovedEvent&);

private:
	EventMgrPtr _eventMgr;
	CameraPtr   _camera;

	bool _move;
	bool _zoom;

	glm::vec2 _cursorPos;
	glm::vec2 _prevPos;

	float _pitch = 0;
	float _yaw   = 0;
	float _dist  = 1.0f;
};
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
