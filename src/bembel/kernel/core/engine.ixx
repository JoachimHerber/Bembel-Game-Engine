module;
#include "bembel/pch.h"
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

    EventManager&         getEventManager() /***/ { return m_event_manager; }
    EventManager const&   getEventManager() const { return m_event_manager; }
    AssetManager&         getAssetManager() /***/ { return m_asset_manager; }
    AssetManager const&   getAssetManager() const { return m_asset_manager; }
    DisplayManager&       getDisplayManager() /***/ { return *m_display_manager; }
    DisplayManager const& getDisplayManager() const { return *m_display_manager; }
    InputManager&         getInputManager() /***/ { return *m_input_manager; }
    InputManager const&   getInputManager() const { return *m_input_manager; }

    template <typename SystemType, typename... Args>
    SystemType* addSystem(Args&&... args) {
        auto system         = std::make_unique<SystemType>(*this, args...);
        auto system_pointer = system.get();

        std::string const& name = system->getName();

        auto it = m_system_mapping.find(name);
        if(it != m_system_mapping.end()) {
            log().error("System '{}' has already been added.", system->getName());
            return nullptr;
        }

        for(size_t n = 0; n < m_systems.size(); ++n) {
            if(!m_systems[n]) {
                m_systems[n] = std::move(system);
                m_system_mapping.emplace(name, n);
                return system_pointer;
            }
        }

        m_system_mapping.emplace(name, m_systems.size());
        m_systems.push_back(std::move(system));
        return system_pointer;
    }
    bool    removeSystem(std::string_view name);
    System* getSystem(std::string_view name);

    bool loadSetting(std::filesystem::path config_file);

    bool initSystems();
    void updateSystems(double time_since_last_update);
    void shutdownSystems();

    void pollEvents();

  private:
    EventManager                    m_event_manager;
    AssetManager                    m_asset_manager;
    std::unique_ptr<DisplayManager> m_display_manager;
    std::unique_ptr<InputManager>   m_input_manager;

    Dictionary<size_t>                   m_system_mapping;
    std::vector<std::unique_ptr<System>> m_systems;
};

} // namespace bembel::kernel
