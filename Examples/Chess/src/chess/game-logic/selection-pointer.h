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

#ifndef BEMBEL_SELECTIONCURSOR_H
#define BEMBEL_SELECTIONCURSOR_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include <bembel-open-gl.h>

#include <bembel-graphics/viewport.h>
#include <bembel-interaction/signal.hpp>
#include <bembel-kernel/events/input-events.h>

#include <glm/glm.hpp>

/*============================================================================*/
/* FORWARD DECLARATIONS                                                       */
/*============================================================================*/
namespace bembel {

class EventManager;
class GraphicSystem;
class Scene;

}//end of namespace bembel
/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
class SelectionPointer
{
public:
	SelectionPointer( bembel::EventManager*, bembel::GraphicSystem*, bembel::Scene* scene );
	~SelectionPointer();

	void HandleEvent( const bembel::CursorMovedEvent& event );
	void HandleEvent( const bembel::MouseButtonPressEvent& event );

	const glm::vec3& GetRayOrigin()   const;
	const glm::vec3& GetRayDirection() const;

	const glm::ivec2& GetSelectedTile() const;

	bembel::Signal<>& GetSelectSignal();

private:
	void UpdateRay( bembel::Viewport::View* view, const glm::vec2& pos );

private:
	bembel::EventManager*   _eventMgr;
	bembel::GraphicSystem*  _grapicSys;
	bembel::Scene*          scene_;

	glm::vec3 _rayOrigin;
	glm::vec3 _rayDirection;

	glm::ivec2 _tile;

	bembel::Signal<> _select;
};

/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
