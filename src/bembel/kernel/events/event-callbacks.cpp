#include "./event-callbacks.hpp"

#include <GLFW/glfw3.h>

#include "../../base/utils/utils.hpp"
#include "../core/kernel.hpp"
#include "../display/display-manager.hpp"
#include "../display/window.hpp"
#include "./display-events.hpp"
#include "./input-events.hpp"

namespace bembel::kernel {
namespace event_callbacks {
    Window* getWindow(GLFWwindow* glfw) {
        return static_cast<Window*>(glfwGetWindowUserPointer(glfw));
    }

    void windowPositionCallback(GLFWwindow* glfw, int x, int y) {
        auto  window    = getWindow(glfw);
        auto& event_mgr = window->getDisplayManager().getEventManager();

        event_mgr.broadcast(WindowMovedEvent{window, glm::vec2(x, y)});
    }
    void windowSizeCallback(GLFWwindow* glfw, int w, int h) {
        auto  window    = getWindow(glfw);
        auto& event_mgr = window->getDisplayManager().getEventManager();

        event_mgr.broadcast(WindowResizeEvent{window, glm::vec2(w, h)});
    }
    void windowCloseCallback(GLFWwindow* glfw) {
        auto  window    = getWindow(glfw);
        auto& event_mgr = window->getDisplayManager().getEventManager();

        event_mgr.broadcast(WindowShouldCloseEvent{window});
    }
    void windowRefreshCallback(GLFWwindow* glfw) {
        auto  window    = getWindow(glfw);
        auto& event_mgr = window->getDisplayManager().getEventManager();
        // currently unused
    }
    void windowFocusCallback(GLFWwindow* glfw, int focused) {
        auto  window    = getWindow(glfw);
        auto& event_mgr = window->getDisplayManager().getEventManager();

        if(focused)
            event_mgr.broadcast(WindowGainedFocusEvent{window});
        else
            event_mgr.broadcast(WindowLostFocusEvent{window});
    }
    void windowIconifyCallback(GLFWwindow* glfw, int iconified) {
        auto  window    = getWindow(glfw);
        auto& event_mgr = window->getDisplayManager().getEventManager();

        if(iconified)
            event_mgr.broadcast(WindowIconifyedEvent{window});
        else
            event_mgr.broadcast(WindowRestoredEvent{window});
    }

    void framebufferSizeCallback(GLFWwindow* glfw, int w, int h) {
        auto  window    = getWindow(glfw);
        auto& event_mgr = window->getDisplayManager().getEventManager();

        glm::vec2 framebuffer_size(w, h);

        event_mgr.broadcast(FrameBufferResizeEvent{window, framebuffer_size});
        window->updateViewports(framebuffer_size);
    }

    void keyCallback(GLFWwindow* glfw, int key_id, int scancode, int action, int mods) {
        auto  window    = getWindow(glfw);
        auto& event_mgr = window->getDisplayManager().getEventManager();

        if(action == GLFW_PRESS)
            event_mgr.broadcast(KeyPressEvent{window, key_id, scancode, mods});
        else if(action == GLFW_REPEAT)
            event_mgr.broadcast(KeyRepeatEvent{window, key_id, scancode, mods});
        else if(action == GLFW_RELEASE)
            event_mgr.broadcast(KeyReleaseEvent{window, key_id, scancode, mods});
    }

    void charCallback(GLFWwindow* glfw, unsigned int c) {
        auto  window    = getWindow(glfw);
        auto& event_mgr = window->getDisplayManager().getEventManager();

        event_mgr.broadcast(TextInputEvent{window, c});
    }
    void charModsCallback(GLFWwindow* glfw, unsigned int c, int mods) {
        auto  window    = getWindow(glfw);
        auto& event_mgr = window->getDisplayManager().getEventManager();
        // currently unused
    }

    void mouseButtonCallback(GLFWwindow* glfw, int button_id, int action, int mods) {
        auto  window    = getWindow(glfw);
        auto& event_mgr = window->getDisplayManager().getEventManager();

        if(action == GLFW_PRESS)
            event_mgr.broadcast(MouseButtonPressEvent{window, button_id, mods});
        else if(action == GLFW_REPEAT)
            event_mgr.broadcast(MouseButtonRepeatEvent{window, button_id, mods});
        else if(action == GLFW_RELEASE)
            event_mgr.broadcast(MouseButtonReleaseEvent{window, button_id, mods});
    }

    void cursorPositionCallback(GLFWwindow* glfw, double x, double y) {
        auto  window    = getWindow(glfw);
        auto& event_mgr = window->getDisplayManager().getEventManager();

        event_mgr.broadcast(CursorMovedEvent{window, glm::vec2(x, y)});

        for(auto& vp : base::reverse(window->getViewports())) {
            if(!vp->isEnabled()) continue;

            glm::vec2 pos = glm::vec2(x, y) - glm::vec2(vp->getPosition());
            pos.y         = vp->getSize().y - pos.y;
            vp->onCurserMove(pos);
        }
    }

    void cursorEnterCallback(GLFWwindow* glfw, int entered) {
        auto  window    = getWindow(glfw);
        auto& event_mgr = window->getDisplayManager().getEventManager();

        if(entered)
            event_mgr.broadcast(CursorEnteredEvent{window});
        else
            event_mgr.broadcast(CursorLeftEvent{window});
    }

    void scrollCallback(GLFWwindow* glfw, double x, double y) {
        auto  window    = getWindow(glfw);
        auto& event_mgr = window->getDisplayManager().getEventManager();

        event_mgr.broadcast(ScrollEvent{window, x, y});
    }

    void dropCallback(GLFWwindow* glfw, int count, const char** files) {
        auto  window    = getWindow(glfw);
        auto& event_mgr = window->getDisplayManager().getEventManager();

        FileDropEvent event;
        event.window = getWindow(glfw);
        for(int i = 0; i < count; ++i) event.files.push_back(files[i]);
        event_mgr.broadcast(event);
    }

    void monitorCallback(GLFWmonitor* glfw, int) {
        //  Window*       window = GetWindow(glfw);
        //  EventManager* eventMgr = window->GetKernel()->GetEventManager();
    }

} // namespace event_callbacks
} // namespace bembel::kernel
