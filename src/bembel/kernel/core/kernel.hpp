#ifndef BEMBEL_KERNEL_CORE_KERNEL_HPP
#define BEMBEL_KERNEL_CORE_KERNEL_HPP

#include <bembel/base/logging/logger.hpp>

#include "../assets/asset-manager.hpp"
#include "../display/display-manager.hpp"
#include "../events/event-manager.hpp"
#include "../input/input-manager.hpp"

namespace bembel::kernel {

class System;

class BEMBEL_API Kernel final {
  public:
    Kernel();
    ~Kernel();

    EventManager&         getEventManager();
    const EventManager&   getEventManager() const;
    AssetManager&         getAssetManager();
    const AssetManager&   getAssetManager() const;
    DisplayManager&       getDisplayManager();
    const DisplayManager& getDisplayManager() const;
    InputManager&         getInputManager();
    const InputManager&   getInputManager() const;

    template <typename SystemType, typename... Args>
    SystemType* addSystem(Args&&... args);
    bool        removeSystem(const std::string& name);
    System*     getSystem(const std::string& name);

    bool loadSetting(const std::string& config_file_name);

    bool initSystems();
    void updateSystems(double time_since_last_update);
    void shutdownSystems();

    void pollEvents();

  private:
    EventManager   event_manager;
    AssetManager   asset_manager;
    DisplayManager display_manager{event_manager, asset_manager};
    InputManager   input_manager{event_manager};

    std::unordered_map<std::string, unsigned> system_mapping;
    std::vector<std::unique_ptr<System>>      systems;
};

template <typename SystemType, typename... Args>
inline SystemType* Kernel::addSystem(Args&&... args) {
    auto system         = std::make_unique<SystemType>(*this, args...);
    auto system_pointer = system.get();

    const std::string& name = system->getName();

    auto it = this->system_mapping.find(name);
    if(it != this->system_mapping.end()) {
        BEMBEL_LOG_ERROR() << "System \"" << system->getName() << "\" has already been added."
                           << std::endl;
        return nullptr;
    }

    for(size_t n = 0; n < this->systems.size(); ++n) {
        if(!this->systems[n]) {
            this->systems[n] = std::move(system);
            this->system_mapping.emplace(name, n);
            return system_pointer;
        }
    }

    this->system_mapping.emplace(name, this->systems.size());
    this->systems.push_back(std::move(system));
    return system_pointer;
}

} // namespace bembel::kernel
#endif // include guards
