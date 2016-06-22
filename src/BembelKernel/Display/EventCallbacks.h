#ifndef BEMBEL_EVENTCALLBACKS_H
#define BEMBEL_EVENTCALLBACKS_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include <BembelConfig.h>

/*============================================================================*/
/* FORWARD DECLARATIONS                                                       */
/*============================================================================*/
struct GLFWwindow;
struct GLFWmonitor;
/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel{
namespace glfw_callbacks{

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
