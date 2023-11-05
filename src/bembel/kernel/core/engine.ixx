module;
#include <string_view>
#include <filesystem>
export module bembel.kernel.core:Engine;

import bembel.base;
import bembel.kernel.assets;
import bembel.kernel.display;
import bembel.kernel.input;

import :System;

namespace bembel::kernel {
using namespace bembel::base;

export class Engine final {
  public:
    Engine();
    Engine(Engine&&)      = delete;
    Engine(Engine const&) = delete;
    ~Engine();

    template <typename SystemType, typename... Args>
    SystemType* addSystem(Args&&... args);
    bool        removeSystem(std::string_view name);
    System*     getSystem(std::string_view name);

    bool loadSetting(std::filesystem::path config_file);

    bool initSystems();
    void updateSystems(double time_since_last_update);
    void shutdownSystems();

    void pollEvents();

  public:
    DisplayManager display;
    InputManager   input;

  private:
    Dictionary<size_t>                   m_system_mapping;
    std::vector<std::unique_ptr<System>> m_systems;
};

template <typename SystemType, typename... Args>
inline SystemType* Engine::addSystem(Args&&... args) {
    auto system         = std::make_unique<SystemType>(*this, args...);
    auto system_pointer = system.get();

    auto it = m_system_mapping.find(system->name);
    if(it != m_system_mapping.end()) {
        logError("System '{}' has already been added.", system->name);
        return nullptr;
    }

    for(size_t n = 0; n < m_systems.size(); ++n) {
        if(!m_systems[n]) {
            m_system_mapping.emplace(system->name, n);
            m_systems[n] = std::move(system);
            return system_pointer;
        }
    }

    m_system_mapping.emplace(system->name, m_systems.size());
    m_systems.push_back(std::move(system));
    return system_pointer;
}

} // namespace bembel::kernel
