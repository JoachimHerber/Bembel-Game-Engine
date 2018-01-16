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

#ifndef BEMBEL_KERNEL_EVENTS_INPUTEVENTS_H_
#define BEMBEL_KERNEL_EVENTS_INPUTEVENTS_H_
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


struct BEMBEL_API KeyPressEvent
{
	Window* const window;
	int key_id;
	int scancode;
	int mods;
};
struct BEMBEL_API KeyRepeatEvent
{
	Window* const window;
	int key_id;
	int scancode;
	int mods;
};
struct BEMBEL_API KeyReleaseEvent
{
	Window* const window;
	int key_id;
	int scancode;
	int mods;
};

struct BEMBEL_API TextInputEvent
{
	Window* const window;
	char32_t      character;
};

struct BEMBEL_API MouseButtonPressEvent
{
	Window* const window;
	int button_id;
	int mods;
};
struct BEMBEL_API MouseButtonRepeatEvent
{
	Window* const window;
	int button_id;
	int mods;
};
struct BEMBEL_API MouseButtonReleaseEvent
{
	Window* const window;
	int button_id;
	int mods;
};

struct BEMBEL_API CursorMovedEvent
{
	Window* const window;
	glm::dvec2 position;
};

struct BEMBEL_API CursorEnteredEvent
{
	Window* const window;
};
struct BEMBEL_API CursorLeftEvent
{
	Window* const window;
};

struct BEMBEL_API ScrollEvent
{
	Window* const window;
	double x;
	double y;
};

struct BEMBEL_API FileDropEvent
{
	Window* window;

	std::vector<std::string> files;
};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
