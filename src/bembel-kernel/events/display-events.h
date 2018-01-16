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

#ifndef BEMBEL_KERNEL_EVENTS_DISPLAYEVENTS_H_
#define BEMBEL_KERNEL_EVENTS_DISPLAYEVENTS_H_
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "bembel-config.h"
#include "event-manager.h"

#include <glm/glm.hpp>

/*============================================================================*/
/* FORWARD DECLARATIONS                                                       */
/*============================================================================*/
namespace bembel {

class Window;

} //end of namespace bembel
/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel {

struct BEMBEL_API WindowOpendEvent
{
	Window* const window;
};
struct BEMBEL_API WindowClosedEvent
{
	Window* const window;
};
struct BEMBEL_API WindowUpdateEvent
{
	Window* const window;
};


struct BEMBEL_API WindowMovedEvent
{
	Window* const window;
	const glm::vec2 position;
};
struct BEMBEL_API WindowResizeEvent
{
	Window* const window;
	const glm::vec2 size;
};

struct BEMBEL_API WindowShouldCloseEvent
{
	Window* const window;
};


struct BEMBEL_API WindowGainedFocusEvent
{
	Window* const window;
};
struct BEMBEL_API WindowLostFocusEvent
{
	Window* const window;
};


struct BEMBEL_API WindowIconifyedEvent
{
	Window* const window;
};
struct BEMBEL_API WindowRestoredEvent
{
	Window* const window;
};


struct BEMBEL_API FrameBufferResizeEvent
{
	Window* const   window;
	const glm::vec2 size;
};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
