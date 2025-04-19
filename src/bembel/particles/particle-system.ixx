export module bembel.particles:System;

import std;
import bembel.base;
import bembel.kernel;
import :Data;

namespace bembel::particles {
using namespace bembel::base;
using namespace bembel::kernel;

export class ParticleSystem : public kernel::System {
  public:
    ParticleSystem(In<Engine*>);
    ParticleSystem(ParticleSystem const&)            = delete;
    ParticleSystem& operator=(ParticleSystem const&) = delete;
    ~ParticleSystem();

    void addScene(std::weak_ptr<Scene> scene);

    std::optional<uint> getParticleTypeIndex(std::string_view type_name) const;

    virtual bool configure(xml::Element const*) override;

    virtual bool init() override;
    virtual void shutdown() override;

    void handleEvent(AppUpdateEvent event);

  private:
    Engine*                           m_engine;
    EventHandlerGuard<AppUpdateEvent> m_guard = {this};

    std::vector<ParticleType>         m_particle_types;
    std::vector<std::weak_ptr<Scene>> m_scenes;
};

} // namespace bembel::particles
