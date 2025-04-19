module;
module bembel.particles;

import std;
import bembel.base;
import bembel.kernel;
import bembel.graphics;

namespace bembel::particles {
using namespace bembel::base;
using namespace bembel::kernel;

void ParticlePopulation::spawnParticleEffect(
    In<ParticleEffect> effect, In<vec3> position, In<quat> rotation, In<float> scale
) {
    mat3 rotate = mat3_cast(rotation);
    for(size_t n = 0; n < effect.getParticleCount(); ++n) {
        vec3 pos   = rotate * effect.getRandomStartPosition();
        vec3 speed = rotate * effect.getRandomStartVelocety();
        pos *= scale;
        pos += position;
        m_particles.emplace_back(pos, effect.getColor(), speed, 0);
    }
}

void ParticlePopulation::update(Seconds time_since_last_update) {
    if(!m_type) return;

    float const ΔT           = float(time_since_last_update.count());
    std::size_t write_offset = 0;
    for(auto& particle : m_particles) {
        if(particle.age > m_type->max_age) continue;

        particle.position += ΔT * particle.velocety;
        particle.velocety += ΔT * m_type->acceleration;
        particle.velocety *= std::pow(1.0f - m_type->drag, ΔT);
        particle.age += ΔT;

        m_particles[write_offset++] = particle;
    }
    m_particles.resize(write_offset);
}

ParticleData::ParticleData(Scene*, std::span<ParticleType> particle_types) {
    for(auto& type : particle_types) { m_particles.emplace_back(&type); }
}
ParticleData::~ParticleData() {}

void ParticleData::update(Seconds ΔT) {
    for(auto& particles : m_particles) { particles.update(ΔT); }
}

void ParticleData::spawnParticleEffect(
    In<ParticleEffect> effect, In<vec3> position, In<quat> rotation, In<float> scale
) {
    if(effect.getParticleType() >= m_particles.size()) return;
    m_particles[effect.getParticleType()].spawnParticleEffect(effect, position, rotation, scale);
}

} // namespace bembel::particles
