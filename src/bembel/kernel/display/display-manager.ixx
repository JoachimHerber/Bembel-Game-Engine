module;
#include <memory>
export module bembel.kernel.display:DisplayManager;

import bembel.base;
import bembel.kernel.assets;

import :Window;
import :DisplayMode;

namespace bembel::kernel {
using namespace bembel::base;

export class DisplayManager final {
  public:
    DisplayManager();
    ~DisplayManager() { deleteAllWindows(); }

    Window* createWindow() {
        m_windows.push_back(std::make_unique<Window>(WindowId(m_windows.size())));
        return m_windows.back().get();
    }
    Window* getWindow(unsigned id) const {
        return (id < m_windows.size()) ? m_windows[id].get() : nullptr;
    }
    usize getNumWindows() const { return usize(m_windows.size()); }

    bool createWindows(const base::xml::Element* properties);

    void closeOpenWindows();
    void deleteAllWindows() {
        closeOpenWindows();
        m_windows.clear();
    }

    void updateWindows();

  private:
    std::vector<std::unique_ptr<Window>> m_windows;

    friend class Window;
};

} // namespace bembel::kernel
