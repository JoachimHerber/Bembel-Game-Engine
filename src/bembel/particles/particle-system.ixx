module;
#include <memory>
#include <optional>
#include <string>
export module bembel.particles:System;

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
    virtual void update(double time_since_last_update) override;

  private:
    Engine* m_engine;

    std::vector<ParticleType>         m_particle_types;
    std::vector<std::weak_ptr<Scene>> m_scenes;
};

} // namespace bembel::particles
