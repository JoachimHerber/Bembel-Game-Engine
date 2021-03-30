#ifndef BEMBEL_KERNEL_DISPLAY_DISPLAYMODE_HPP
#define BEMBEL_KERNEL_DISPLAY_DISPLAYMODE_HPP

#include <bembel/base/io/xml.hpp>

struct GLFWwindow;

namespace bembel::kernel {

class BEMBEL_API DisplayModeBase {
  public:
    DisplayModeBase()          = default;
    virtual ~DisplayModeBase() = default;

    virtual GLFWwindow* creatWindow(const std::string&, GLFWwindow*) = 0;
};

class BEMBEL_API WindowDisplayMode : public DisplayModeBase {
  public:
    WindowDisplayMode()  = default;
    ~WindowDisplayMode() = default;

    int getWidth() const;
    void setWidth(int);

    int getHeight() const;
    void setHeight(int);

    bool getIsResizable() const;
    void setIsResizable(bool);

    GLFWwindow* creatWindow(const std::string&, GLFWwindow*) override;

    static std::unique_ptr<WindowDisplayMode> createInstance(const base::xml::Element*);

    static const std::string& getTypeName();

  private:
    int width  = 800;
    int height = 600;

    bool limit_size = false;
    glm::uvec2 min_size;
    glm::uvec2 max_size;

    unsigned aspect_ratio_numer = 0;
    unsigned aspect_ratio_denom = 0;

    bool resizable = true;
};

class BEMBEL_API FullscreenDisplayMode : public DisplayModeBase {
  public:
    FullscreenDisplayMode();
    ~FullscreenDisplayMode() = default;

    glm::ivec2 getResolution() const;
    void setResolution(glm::ivec2 val);

    int getRefreshRate() const;
    void setRefreshRate(int val);

    GLFWwindow* creatWindow(const std::string&, GLFWwindow*) override;

    static std::unique_ptr<FullscreenDisplayMode> createInstance(const base::xml::Element*);

    static const std::string& getTypeName();

  private:
    glm::ivec2 resolution;

    int refresh_rate;
};

} // namespace bembel::kernel
#endif // include guards
