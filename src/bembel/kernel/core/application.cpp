#include "./application.hpp"

#include <bembel/base/logging/logger.hpp>

#include "../display/display-manager.hpp"
#include "../events/event-manager.hpp"
#include "./kernel.hpp"

namespace bembel::kernel {

Application::Application()
: kernel(std::make_unique<Kernel>()) {
}

Application::~Application() {
}

bool Application::run() {
    {
        BEMBEL_LOG_INFO() << "Initalizing Application";
        base::IndentDefaultLogs indent;
        if(!this->init()) return false;
    }
    {
        BEMBEL_LOG_INFO() << "Starting MainLoop";
        base::IndentDefaultLogs indent;
        this->mainLoop();
    }
    {
        BEMBEL_LOG_INFO() << "Cleaning up";
        base::IndentDefaultLogs indent;
        this->cleanup();
    }
    return true;
}

void Application::quit() {
    this->should_exit_main_loop = true;
}

void Application::mainLoop() {
    auto time = std::chrono::high_resolution_clock::now();

    this->should_exit_main_loop = false;
    while(!this->should_exit_main_loop) {
        auto                      now = std::chrono::high_resolution_clock::now();
        std::chrono::milliseconds ms =
            std::chrono::duration_cast<std::chrono::milliseconds>(now - time);

        double timeSinceLastUpdate = 0.001 * (ms.count());

        this->kernel->pollEvents();
        update(timeSinceLastUpdate);
        this->kernel->updateSystems(timeSinceLastUpdate);
        this->kernel->getDisplayManager().updateWindows();

        time = now;
    }
}

} // namespace bembel::kernel
