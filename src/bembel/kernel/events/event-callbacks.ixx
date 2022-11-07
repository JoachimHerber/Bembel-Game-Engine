export module bembel.kernel : EventCallbacks;

struct GLFWwindow;
struct GLFWmonitor;

namespace bembel::kernel {

export namespace event_callbacks {

    void windowPositionCallback(GLFWwindow*, int, int);
    void windowSizeCallback(GLFWwindow*, int, int);
    void windowCloseCallback(GLFWwindow*);
    void windowRefreshCallback(GLFWwindow*);
    void windowFocusCallback(GLFWwindow*, int);
    void windowIconifyCallback(GLFWwindow*, int);

    void framebufferSizeCallback(GLFWwindow*, int, int);

    void keyCallback(GLFWwindow*, int, int, int, int);
    void charCallback(GLFWwindow*, unsigned int);
    void charModsCallback(GLFWwindow*, unsigned int, int);

    void mouseButtonCallback(GLFWwindow*, int, int, int);
    void cursorPositionCallback(GLFWwindow*, double, double);
    void cursorEnterCallback(GLFWwindow*, int);
    void scrollCallback(GLFWwindow*, double, double);

    void dropCallback(GLFWwindow*, int, const char**);

    void monitorCallback(GLFWmonitor*, int);

} // namespace event_callbacks

} // namespace bembel::kernel
