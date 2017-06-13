/******************************************************************************/
/* ************************************************************************** */
/* *                                                                        * */
/* *    MIT License                                                         * */
/* *                                                                        * */
/* *   Copyright(c) 2017 Joachim Herber                                     * */
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

#ifndef BEMBEL_KERNEL_EVENTS_EVENTCALLBACKS_H_
#define BEMBEL_KERNEL_EVENTS_EVENTCALLBACKS_H_
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "bembel-config.h"

/*============================================================================*/
/* FORWARD DECLARATIONS                                                       */
/*============================================================================*/
struct GLFWwindow;
struct GLFWmonitor;
/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel {
namespace glfw_callbacks {

void WindowPositionCallback(GLFWwindow*, int, int);
void WindowSizeCallback(GLFWwindow*, int, int);
void WindowCloseCallback(GLFWwindow*);
void WindowRefreshCallback(GLFWwindow*);
void WindowFocusCallback(GLFWwindow*, int);
void WindowIconifyCallback(GLFWwindow*, int);

void FramebufferSizeCallback(GLFWwindow*, int, int);

void KeyCallback(GLFWwindow*, int, int, int, int);
void CharCallback(GLFWwindow*, unsigned int);
void CharModsCallback(GLFWwindow*, unsigned int, int);

void MouseButtonCallback(GLFWwindow*, int, int, int);
void CursorPositionCallback(GLFWwindow*, double, double);
void CursorEnterCallback(GLFWwindow*, int);
void ScrollCallback(GLFWwindow*, double, double);

void DropCallback(GLFWwindow*, int, const char**);

void MonitorCallback(GLFWmonitor*, int);

} //end of namespace event_callbacks
} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
