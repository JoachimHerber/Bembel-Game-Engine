export module bembel.kernel.core:Application;

import std;
import bembel.base;

import :Engine;

namespace bembel::kernel {
using namespace bembel::base;

export struct FrameStartEvent {};
export struct AppUpdateEvent {
    Seconds ΔT;
};
export struct AppRenderEvent {};
export struct FrameEndEvent {};

export template <typename... TSystems>
class Application {
  public:
    Application() : m_systems{m_engine.addSystem<TSystems>()...} {}
    virtual ~Application() = default;

    bool run(std::span<std::string_view> args) {
        {
            logInfo("Initalizing Application");
            IndentDefaultLogs indent;
            if(!init(args)) return false;
        }
        {
            logInfo("Starting MainLoop");
            IndentDefaultLogs indent;
            mainLoop();
        }
        {
            logInfo("Cleaning up");
            IndentDefaultLogs indent;
            cleanup();
        }
        return true;
    }
    void quit() { m_should_exit_main_loop = true; }

    template <typename TSystem>
    TSystem* getSystem() const {
        return std::get<TSystem*>(m_systems);
    }

  protected:
    virtual void mainLoop() {
        using namespace std::chrono;

        auto time = high_resolution_clock::now();

        m_should_exit_main_loop = false;
        while(!m_should_exit_main_loop) {
            auto const now = high_resolution_clock::now();
            update(duration_cast<Seconds>(now - time));
            time = now;
        }
    }

    virtual bool init(std::span<std::string_view> args) = 0;
    virtual void cleanup()                              = 0;

    virtual void update(Seconds const ΔT) {
        events::broadcast<FrameStartEvent>();
        m_engine.pollEvents();
        events::broadcast<AppUpdateEvent>(ΔT);
        events::broadcast<AppRenderEvent>();
        m_engine.display.updateWindows();
        events::broadcast<FrameEndEvent>();
    }

  protected:
    Engine m_engine;

  private:
    std::tuple<TSystems*...> m_systems;
    bool                     m_should_exit_main_loop = false;
};

} // namespace bembel::kernel
