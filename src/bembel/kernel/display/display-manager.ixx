module;
#include "bembel/pch.h"
export module bembel.kernel.display : DisplayManager;

import bembel.base;
import bembel.kernel.assets;

import : Window;
import : DisplayMode;

namespace bembel::kernel {
using namespace bembel::base;

export class DisplayManager final {
  public:
    DisplayManager(EventManager& event_mgr, AssetManager& asset_mgr);
    ~DisplayManager() { deleteAllWindows(); }

    Window* createWindow() {
        m_windows.push_back(std::make_unique<Window>(m_event_mgr, WindowId(m_windows.size())));
        return m_windows.back().get();
    }
    Window* getWindow(unsigned id) const {
        if(id < m_windows.size()) return m_windows[id].get();

        return nullptr;
    }
    unsigned      getNumWindows() const { return unsigned(m_windows.size()); }

    EventManager& getEventManager() const { return m_event_mgr; }

    bool          createWindows(const base::xml::Element* properties);

    void          closeOpenWindows();
    void          deleteAllWindows() {
        closeOpenWindows();
        m_windows.clear();
    }

    void updateWindows();

  private:
    EventManager&                        m_event_mgr;

    std::vector<std::unique_ptr<Window>> m_windows;

    friend class Window;
};

} // namespace bembel::kernel
