/******************************************************************************/
/* ************************************************************************** */
/* *                                                                        * */
/* *    MIT License                                                         * */
/* *                                                                        * */
/* *   Copyright(c) 2018 Joachim Herber                                     * */
/* *                                                                        * */
/* *   Permission is hereby granted, free of charge, to any person          * */
/* *   obtaining copy of this software and associated documentation files   * */
/* *   (the "Software"), to deal in the Software without restriction,       * */
/* *   including without limitation the rights to use, copy, modify, merge, * */
/* *   publish, distribute, sublicense, and/or sell copies of the Software, * */
/* *   and to permit persons to whom the Software is furnished to do so,    * */
/* *   subject to the following conditions :                                * */
/* *                                                                        * */
/* *   The above copyright notice and this permission notice shall be       * */
/* *   included in all copies or substantial portions of the Software.      * */
/* *                                                                        * */
/* *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,      * */
/* *   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF   * */
/* *   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND                * */
/* *   NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS   * */
/* *   BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN   * */
/* *   ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN    * */
/* *   CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE     * */
/* *   SOFTWARE.                                                            * */
/* *                                                                        * */
/* ************************************************************************** */
/******************************************************************************/

/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "camera-controle.h"

#include <glm/gtc/quaternion.hpp>
#include <glfw/glfw3.h>
#include <iostream>

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
CameraControle::CameraControle( 
	bembel::EventManager* eventMgr, 
	std::shared_ptr<bembel::Camera> camera )
	: _eventMgr( eventMgr )
	, _camera( camera )
{
	_eventMgr->AddHandler<bembel::MouseButtonPressEvent>( this );
	_eventMgr->AddHandler<bembel::MouseButtonReleaseEvent>( this );
	_eventMgr->AddHandler<bembel::CursorMovedEvent>( this );
	_eventMgr->AddHandler<bembel::ScrollEvent>( this );

	_move = false;
}

CameraControle::~CameraControle()
{
	_eventMgr->RemoveHandler<bembel::MouseButtonPressEvent>( this );
	_eventMgr->RemoveHandler<bembel::MouseButtonReleaseEvent>( this );
	_eventMgr->RemoveHandler<bembel::CursorMovedEvent>( this );
}

void CameraControle::SetPitch( float f )
{
	_pitch = f;
}

void CameraControle::SetYaw( float f )
{
	_yaw = f;
}

void CameraControle::SetDist( float f )
{
	_dist = f;
}

float CameraControle::GetPitch() const
{
	return _pitch;
}

float CameraControle::GetYaw() const
{
	return _yaw;
}

float CameraControle::GetDist() const
{
	return _dist;
}

void CameraControle::EnableManualControle( bool b )
{
	_enabeld = b;
}

void CameraControle::SetCameraOffset( const glm::vec3& pos )
{
	_offset = pos;
}

void CameraControle::Update( double dTime )
{
	glm::vec2 _cursorMovement = _cursorPos-_prevPos;
	_prevPos = _cursorPos;

	if( _enabeld )
	{
		if( _move )
		{
			_yaw -= 0.01f*(_cursorMovement.x);
			_pitch -= 0.01f*(_cursorMovement.y);
			if( _pitch <= -1.5f ) _pitch = -1.5f;
			if( _pitch >= +1.5f ) _pitch = +1.5f;
		}
	}


	const static glm::vec3 X_AXIS = glm::vec3( 1, 0, 0 );
	const static glm::vec3 Y_AXIS = glm::vec3( 0, 1, 0 );
	const static glm::vec3 Z_AXIS = glm::vec3( 0, 0, 1 );

	glm::quat pitch = glm::angleAxis( _pitch, X_AXIS );
	glm::quat yaw = glm::angleAxis( _yaw, Y_AXIS );
	_camera->SetOrientation( yaw*pitch );

	glm::vec3 pos = _offset + _dist*glm::mat3_cast( yaw*pitch)*Z_AXIS;
	_camera->SetPosition( pos );
}

void CameraControle::HandleEvent( const bembel::MouseButtonPressEvent& event )
{
	if( event.button_id == GLFW_MOUSE_BUTTON_RIGHT )
		_move = true;
}

void CameraControle::HandleEvent( const bembel::MouseButtonReleaseEvent& event )
{
	if( event.button_id == GLFW_MOUSE_BUTTON_RIGHT )
		_move = false;
}

void CameraControle::HandleEvent( const bembel::CursorMovedEvent& event )
{
	_cursorPos = event.position;
}

void CameraControle::HandleEvent( const bembel::ScrollEvent& event )
{
	_dist += event.y;
	if( _dist<1 )
		_dist = 1.0f;
}
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/