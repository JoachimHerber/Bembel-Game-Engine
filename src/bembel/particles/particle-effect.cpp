module bembel.particles;

import std;
import bembel.base;
import bembel.kernel;

namespace bembel::particles {
using namespace bembel::base;
using namespace bembel::kernel;

ParticleEffect::ParticleEffect(uint type, uint count, xml::Element const* properties)
  : m_particle_type{type}, m_particle_count{count} {
    xml::getAttribute(properties, "position", m_position.center);
    xml::getAttribute(properties, "position_diviation", m_position.diviation);
    xml::getAttribute(properties, "velocety", m_velocety.center);
    xml::getAttribute(properties, "velocety_diviation", m_velocety.diviation);
}

inline float generateRandomFloat() {
    static std::default_random_engine         generator;
    static std::uniform_int_distribution<int> distribution(-1.f, 1.f);
    return distribution(generator);
}

vec3 ParticleEffect::RandomDistribution::get() const {
    vec3 v = center;
    switch(shape) {
        case BOX: {
            v.x += diviation.x * generateRandomFloat();
            v.y += diviation.y * generateRandomFloat();
            v.z += diviation.z * generateRandomFloat();
            break;
        }
        case SPHERE: {
            float y = generateRandomFloat();
            float φ = generateRandomFloat() * 3.14159265359f;
            float r = std::sqrt(1.0f - y * y);
            v.x += diviation.x * r * std::sin(φ);
            v.y += diviation.y * y;
            v.z += diviation.z * r * std::cos(φ);
            break;
        }
    }
    return v;
}

} // namespace bembel::particles
