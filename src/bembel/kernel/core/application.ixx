﻿module;
#include <chrono>
export module bembel.kernel.core:Application;

import bembel.base;

import :Engine;

namespace bembel::kernel {
using namespace bembel::base;

export class Application {
  public:
    Application() {}
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

  protected:
    virtual void mainLoop() {
        using namespace std::chrono;

        auto time = high_resolution_clock::now();

        m_should_exit_main_loop = false;
        while(!m_should_exit_main_loop) {
            auto         now = high_resolution_clock::now();
            milliseconds ms  = duration_cast<milliseconds>(now - time);

            double timeSinceLastUpdate = 0.001 * (ms.count());

            m_engine.pollEvents();
            update(timeSinceLastUpdate);
            m_engine.updateSystems(timeSinceLastUpdate);
            m_engine.display.updateWindows();

            time = now;
        }
    }

    virtual bool init(std::span<std::string_view> args) = 0;
    virtual void cleanup() = 0;

    virtual void update(double timeDelta) = 0;

  protected:
    Engine m_engine;

  private:
    bool m_should_exit_main_loop = false;
};

} // namespace bembel::kernel
