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

	void EnableManualControle(bool b);

	void SetCameraOffset(const glm::vec3&);

	void Update(double dTime);

	void HandleEvent(const bembel::MouseButtonPressEvent&);
	void HandleEvent(const bembel::MouseButtonReleaseEvent&);
	void HandleEvent(const bembel::CursorMovedEvent&);
	void HandleEvent( const bembel::ScrollEvent& );

private:
	bembel::EventManager*           _eventMgr;
	std::shared_ptr<bembel::Camera> _camera;

	bool _enabeld = false;

	bool _move;

	glm::vec2 _cursorPos;
	glm::vec2 _prevPos;

	float _pitch = -0.1f;
	float _yaw   = 0;
	float _dist  = 10.0f;

	glm::vec3 _offset;
};
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
