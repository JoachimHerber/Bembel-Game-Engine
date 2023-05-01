module;
#include <string_view>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
export module bembel.kernel.display:DisplayMode;

import bembel.base;

namespace bembel::kernel {
using namespace base;

export class DisplayModeBase {
  public:
    DisplayModeBase()          = default;
    virtual ~DisplayModeBase() = default;

    virtual bool configure(xml::Element const*) = 0;

    virtual GLFWwindow* creatWindow(std::string_view, GLFWwindow*) = 0;

    static Factory<DisplayModeBase>& getFactory();
};

export class WindowDisplayMode : public DisplayModeBase {
  public:
    WindowDisplayMode() = default;
    WindowDisplayMode(In<uvec2> size, bool resizable)
      : m_width{size.x}, m_height{size.y}, m_resizable{resizable} {}
    ~WindowDisplayMode() = default;

    u32  getWidth() const { return m_width; }
    void setWidth(u32 value) { m_width = value; }

    u32  getHeight() const { return m_height; }
    void setHeight(u32 value) { m_height = value; }

    bool getIsResizable() const { return m_resizable; }
    void setIsResizable(bool value) { m_resizable = value; }

    void setAspectRatio(u32 numer, u32 denom) {
        m_aspect_ratio_numer = numer;
        m_aspect_ratio_denom = denom;
    }

    virtual bool configure(xml::Element const*) override;

    GLFWwindow* creatWindow(std::string_view, GLFWwindow*) override;

    static constexpr std::string_view TYPE_NAME = "WindowDisplayMode";

  private:
    u32 m_width  = 800;
    u32 m_height = 600;

    bool  m_limit_size = false;
    uvec2 m_min_size;
    uvec2 m_max_size;

    u32 m_aspect_ratio_numer = 0;
    u32 m_aspect_ratio_denom = 0;

    bool m_resizable = true;

    static bool registerd;
};

export class FullscreenDisplayMode : public DisplayModeBase {
  public:
    FullscreenDisplayMode();
    ~FullscreenDisplayMode() = default;

    ivec2 getResolution() const { return m_resolution; }
    void  setResolution(ivec2 value) { m_resolution = value; }

    int  getRefreshRate() const { return m_refresh_rate; }
    void setRefreshRate(int value) { m_refresh_rate = value; }

    virtual bool configure(xml::Element const*) override;

    GLFWwindow* creatWindow(std::string_view, GLFWwindow*) override;

    static constexpr std::string_view TYPE_NAME = "FullscreenDisplayMode";

  private:
    ivec2 m_resolution;

    int m_refresh_rate;

    static bool registerd;
};

} // namespace bembel::kernel
