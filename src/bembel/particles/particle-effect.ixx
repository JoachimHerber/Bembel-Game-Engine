module;
#include <string_view>
export module bembel.particles:Effects;

import bembel.base;
import bembel.kernel;

namespace bembel::particles {
using namespace bembel::base;
using namespace bembel::kernel;

export class ParticleEffect {
  public:
    ParticleEffect(uint type, uint count, xml::Element const* properties);
    ParticleEffect(ParticleEffect const&) = delete;
    ParticleEffect(ParticleEffect&&)      = delete;
    ~ParticleEffect()                     = default;

    struct RandomDistribution {
        enum Shape { BOX, SPHERE };
        Shape shape;
        vec3  center;
        vec3  diviation;

        vec3 get() const;
    };

    uint getParticleType() const { return m_particle_type; }
    uint getParticleCount() const { return m_particle_count; }

    ColorRGBA getColor() const { return m_color; }

    vec3 getRandomStartPosition() const { return m_position.get(); };
    vec3 getRandomStartVelocety() const { return m_velocety.get(); };

    static constexpr std::string_view ASSET_TYPE_NAME = "ParticleEffect";

  private:
    uint               m_particle_type  = 0;
    uint               m_particle_count = 1;
    ColorRGBA          m_color          = {0xFF, 0xFF, 0xFF, 0xFF};
    RandomDistribution m_position       = {RandomDistribution::BOX, vec3{0, 0, 0}, vec3{0, 0, 0}};
    RandomDistribution m_velocety = {RandomDistribution::SPHERE, vec3{0, 0, 0}, vec3{1, 1, 1}};
};

} // namespace bembel::particles
