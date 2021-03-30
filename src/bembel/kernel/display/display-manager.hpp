#ifndef BEMBEL_KERNEL_DISPLAY_DISPLAYMANAGER_HPP
#define BEMBEL_KERNEL_DISPLAY_DISPLAYMANAGER_HPP

#include <bembel/base/io/xml.hpp>
#include <bembel/base/utils/factory.hpp>
#include <bembel/kernel/assets/asset-manager.hpp>
#include <bembel/kernel/events/event-manager.hpp>

#include "./display-mode.hpp"

struct GLFWwindow;

namespace bembel::kernel {

class Kernel;
class Window;

class BEMBEL_API DisplayManager {
  public:
    DisplayManager(EventManager& event_mgr, AssetManager& asset_mgr);
    ~DisplayManager();

    using DisplayModeFactory = base::Factory<DisplayModeBase, const base::xml::Element*>;

    Window*  createWindow();
    Window*  getWindow(unsigned id) const;
    unsigned getNumWindows() const;

    EventManager&       getEventManager() const;
    DisplayModeFactory& getDisplayModeFactory();

    bool createWindows(const base::xml::Element* properties);
    void closeOpenWindows();
    void deleteAllWindows();

    void updateWindows();

  public:
    void onWindowOpend(Window*);
    void onWindowClosed(Window*);

  private:
    EventManager& event_mgr;

    std::vector<std::unique_ptr<Window>> windows;
    std::vector<Window*>                 open_windows;

    DisplayModeFactory display_mode_factory;

    friend class Window;
};

} // namespace bembel::kernel
#endif // include guards
