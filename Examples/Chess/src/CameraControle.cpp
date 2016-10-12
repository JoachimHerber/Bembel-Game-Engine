#include "CameraControle.h"

#include <glm/gtx/quaternion.hpp>
#include <glfw/glfw3.h>
#include <iostream>

CameraControle::CameraControle(EventMgrPtr eventMgr, CameraPtr camera)
	: _eventMgr(eventMgr)
	, _camera(camera)
{
	_eventMgr->AddHandler<bembel::MouseButtonPressEvent>(this);
	_eventMgr->AddHandler<bembel::MouseButtonReleaseEvent>(this);
	_eventMgr->AddHandler<bembel::CursorMovedEvent>(this);

	_move = false;
	_zoom = false;
}

CameraControle::~CameraControle()
{
	_eventMgr->RemoveHandler<bembel::MouseButtonPressEvent>(this);
	_eventMgr->RemoveHandler<bembel::MouseButtonReleaseEvent>(this);
	_eventMgr->RemoveHandler<bembel::CursorMovedEvent>(this);
}

void CameraControle::Update(double dTime)
{
	glm::vec2 _cursorMovement = _cursorPos-_prevPos;
	_prevPos = _cursorPos;

	if (_move)
	{
		_yaw   -= 0.01f*(_cursorMovement.x);
		_pitch -= 0.01f*(_cursorMovement.y);
		if (_pitch <= -1.5f) _pitch = -1.5f;
		if (_pitch >= +1.5f) _pitch = +1.5f;
	}
	if (_zoom)
	{
		_dist += 0.1f*(_cursorMovement.y);
	}

	const static glm::vec3 X_AXIS = glm::vec3(1, 0, 0);
	const static glm::vec3 Y_AXIS = glm::vec3(0, 1, 0);
	const static glm::vec3 Z_AXIS = glm::vec3(0, 0, 1);

	glm::quat pitch = glm::angleAxis(_pitch, X_AXIS);
	glm::quat yaw   = glm::angleAxis(_yaw,Y_AXIS);
	_camera->SetOrientation(yaw*pitch);

	glm::vec3 pos = _dist*glm::rotate(yaw*pitch, Z_AXIS);
	_camera->SetPosition(pos);
}

void CameraControle::HandleEvent(const bembel::MouseButtonPressEvent& event)
{
	if (event.buttonID == GLFW_MOUSE_BUTTON_LEFT)
		_move = true;
	if (event.buttonID == GLFW_MOUSE_BUTTON_RIGHT)
		_zoom = true;
}

void CameraControle::HandleEvent(const bembel::MouseButtonReleaseEvent& event)
{
	if (event.buttonID == GLFW_MOUSE_BUTTON_LEFT)
		_move = false;
	if (event.buttonID == GLFW_MOUSE_BUTTON_RIGHT)
		_zoom = false;
}

void CameraControle::HandleEvent(const bembel::CursorMovedEvent& event)
{
	_cursorPos = event.position;
}

