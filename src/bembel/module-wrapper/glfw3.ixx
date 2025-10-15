module;
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
export module glfw;

export namespace glfw {
    constexpr auto init      = ::glfwInit;
    constexpr auto terminate = ::glfwTerminate;
    constexpr auto pollEvents = ::glfwPollEvents;

    constexpr auto setDefaultWindowHints = ::glfwDefaultWindowHints;
    constexpr auto setWindowHint         = ::glfwWindowHint;
    constexpr auto createWindow          = ::glfwCreateWindow;
    constexpr auto createStandardCursor  = ::glfwCreateStandardCursor;
    constexpr auto setWindowSizeLimits   = ::glfwSetWindowSizeLimits;
    constexpr auto setWindowAspectRatio  = ::glfwSetWindowAspectRatio;
    constexpr auto getVideoMode          = ::glfwGetVideoMode;
    constexpr auto getPrimaryMonitor     = ::glfwGetPrimaryMonitor;
    constexpr auto getWindowUserPointer  = ::glfwGetWindowUserPointer;
    constexpr auto getProcAddress        = ::glfwGetProcAddress;
    constexpr auto setWindowUserPointer  = ::glfwSetWindowUserPointer;
    constexpr auto destroyWindow         = ::glfwDestroyWindow;

    constexpr auto setWindowPosCallback       = ::glfwSetWindowPosCallback;
    constexpr auto setWindowSizeCallback      = ::glfwSetWindowSizeCallback;
    constexpr auto setWindowCloseCallback     = ::glfwSetWindowCloseCallback;
    constexpr auto setWindowRefreshCallback   = ::glfwSetWindowRefreshCallback;
    constexpr auto setWindowFocusCallback     = ::glfwSetWindowFocusCallback;
    constexpr auto setWindowIconifyCallback   = ::glfwSetWindowIconifyCallback;
    constexpr auto setFramebufferSizeCallback = ::glfwSetFramebufferSizeCallback;
    constexpr auto setKeyCallback             = ::glfwSetKeyCallback;
    constexpr auto setCharCallback            = ::glfwSetCharCallback;
    constexpr auto setCharModsCallback        = ::glfwSetCharModsCallback;
    constexpr auto setMouseButtonCallback     = ::glfwSetMouseButtonCallback;
    constexpr auto setCursorPosCallback       = ::glfwSetCursorPosCallback;
    constexpr auto setCursorEnterCallback     = ::glfwSetCursorEnterCallback;
    constexpr auto setScrollCallback          = ::glfwSetScrollCallback;
    constexpr auto setDropCallback            = ::glfwSetDropCallback;
    constexpr auto setErrorCallback           = ::glfwSetErrorCallback;

    constexpr auto getWindowShouldClose = ::glfwWindowShouldClose;
    constexpr auto setWindowShouldClose = ::glfwSetWindowShouldClose;

    constexpr auto getWindowPos  = ::glfwGetWindowPos;
    constexpr auto setWindowPos  = ::glfwSetWindowPos;
    constexpr auto getWindowSize = ::glfwGetWindowSize;
    constexpr auto setWindowSize = ::glfwSetWindowSize;

    constexpr auto getKeyName = ::glfwGetKeyName;

    constexpr auto getFramebufferSize = ::glfwGetFramebufferSize;
    constexpr auto iconifyWindow      = ::glfwIconifyWindow;
    constexpr auto maximizeWindow     = ::glfwMaximizeWindow;
    constexpr auto restoreWindow      = ::glfwRestoreWindow;
    constexpr auto showWindow         = ::glfwShowWindow;
    constexpr auto hideWindow         = ::glfwHideWindow;

    constexpr auto makeContextCurrent = ::glfwMakeContextCurrent;
    constexpr auto swapBuffers        = ::glfwSwapBuffers;

    constexpr auto setCursor    = ::glfwSetCursor;
    constexpr auto setInputMode = ::glfwSetInputMode;

    using Window    = GLFWwindow;
    using VideoMode = GLFWvidmode;
    using Monitor   = GLFWmonitor;

    constexpr auto RESIZABLE    = GLFW_RESIZABLE;
    constexpr auto SRGB_CAPABLE = GLFW_SRGB_CAPABLE;
    constexpr auto DECORATED    = GLFW_DECORATED;
    constexpr auto RED_BITS     = GLFW_RED_BITS;
    constexpr auto GREEN_BITS   = GLFW_GREEN_BITS;
    constexpr auto BLUE_BITS    = GLFW_BLUE_BITS;
    constexpr auto ALPHA_BITS   = GLFW_ALPHA_BITS;
    constexpr auto REFRESH_RATE = GLFW_REFRESH_RATE;

    constexpr auto PRESS       = GLFW_PRESS;
    constexpr auto REPEAT      = GLFW_REPEAT;
    constexpr auto RELEASE     = GLFW_RELEASE;
    constexpr auto KEY_UNKNOWN = GLFW_KEY_UNKNOWN;

    constexpr auto CURSOR          = GLFW_CURSOR;
    constexpr auto CURSOR_NORMAL   = GLFW_CURSOR_NORMAL;
    constexpr auto CURSOR_HIDDEN   = GLFW_CURSOR_HIDDEN;
    constexpr auto CURSOR_DISABLED = GLFW_CURSOR_DISABLED;

    constexpr auto ARROW_CURSOR     = GLFW_ARROW_CURSOR;
    constexpr auto IBEAM_CURSOR     = GLFW_IBEAM_CURSOR;
    constexpr auto CROSSHAIR_CURSOR = GLFW_CROSSHAIR_CURSOR;
    constexpr auto HAND_CURSOR      = GLFW_HAND_CURSOR;
    constexpr auto HRESIZE_CURSOR   = GLFW_HRESIZE_CURSOR;
    constexpr auto VRESIZE_CURSOR   = GLFW_VRESIZE_CURSOR;

} // namespace glfw

