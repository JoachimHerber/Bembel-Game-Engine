#ifndef BEMBEL_KERNEL_DISPLAY_WINDOW_HPP
#define BEMBEL_KERNEL_DISPLAY_WINDOW_HPP

#include <bembel/base/io/xml.hpp>
#include <bembel/base/utils/signal.hpp>

#include "./viewport.hpp"

struct GLFWwindow;

namespace bembel::kernel {
class DisplayManager;
class DisplayModeBase;
struct SetCursorIconEvent;

class BEMBEL_API Window {
  public:
    Window(DisplayManager&, unsigned);
    Window(const Window&) = delete;
    Window(Window&&)      = default;
    ~Window();

    std::shared_ptr<DisplayModeBase> getDisplayMode() const;
    void                             setDisplayMode(std::shared_ptr<DisplayModeBase> val);

    void init(const base::xml::Element* properties);

    void open(const std::string& titel);
    void close();

    unsigned getWindowID() const;

    bool isOpend();

    glm::ivec2 getWindowSize() const;
    glm::ivec2 getFrameBufferSize() const;

    bool setIconify(bool b);
    bool setVisible(bool b);

    bool getShouldClose() const;
    bool setShouldClose(bool should_close);

    const glm::vec3& getBackgroundColor() const { return this->background_color; }
    void             setBackgroundColor(const glm::vec3& color) { this->background_color = color; }

    Viewport* getViewport(size_t index = 0) const;

    const std::vector<std::unique_ptr<Viewport>>& getViewports() const;

    DisplayManager& getDisplayManager() const;
    GLFWwindow*     getGlfwWindow() const;

    void makeContextCurent();
    void swapBuffers();

    void updateViewports(const glm::ivec2& frambuffer_size);

    void handleEvent(const SetCursorIconEvent& event);

  private:
    DisplayManager&                  display_mgr;
    GLFWwindow*                      window_imp;
    std::shared_ptr<DisplayModeBase> display_mode;

    glm::vec3 background_color = {0.5f, 0.5f, 0.5f};

    std::vector<std::unique_ptr<Viewport>> viewports;

    unsigned window_id;

    friend class DisplayManager;
};

} // namespace bembel::kernel
#endif // include guards
