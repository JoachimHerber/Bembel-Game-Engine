module;
#include <algorithm>
#include <array>
#include <chrono>
#include <span>
#include <vector>
export module bembel.particles:Data;

import bembel.base;
import bembel.kernel;
import :Effects;

namespace bembel::particles {
using namespace bembel::base;
using namespace bembel::kernel;

export struct Particel {
    vec3      position; //   12 Byte
    ColorRGBA color;    // +  4 Byte -> 16 Byte;
    vec3      velocety; // + 12 Byte -> 28 Byte;
    float     age;      // + 4 Byte -> 32 Byte;
};

export struct ParticleType {
    std::string          name;
    float                max_age;
    vec3                 acceleration;
    float                drag;
    std::array<float, 3> size_polinom;
    std::vector<vec4>    tex_coords;

    float calculateParticleSize(float f) const {
        return size_polinom[0] + f * (size_polinom[1] + f * size_polinom[2]);
    }

    vec4 calculateTexturCoords(float f) const {
        if(tex_coords.empty()) return {0.f, 0.f, 1.f, 1.f};
        return tex_coords[std::clamp(
            std::size_t(f * tex_coords.size()), size_t(0), tex_coords.size() - 1
        )];
    }
};

export class ParticlePopulation {
  public:
    ParticlePopulation(ParticleType* type) : m_type{type} {}

    void update(float time_since_last_update);

    std::span<const Particel> getParticles() /**/ const { return m_particles; }
    ParticleType*             getType() const { return m_type; }

    void spawnParticleEffect(
        In<ParticleEffect> effect, In<vec3> position, In<quat> rotation, In<float> scale
    );

  private:
    ParticleType*         m_type;
    std::vector<Particel> m_particles;
};

export class ParticleData : public SceneDataContainerBase {
  public:
    ParticleData(Scene*, std::span<ParticleType> particle_types);
    ~ParticleData();

    void update(float time_since_last_update);

    void spawnParticleEffect(
        In<ParticleEffect> effect, In<vec3> position, In<quat> rotation, In<float> scale
    );

    std::span<const ParticlePopulation> getParticlePopulations() const { return m_particles; }

  private:
    std::vector<ParticlePopulation> m_particles;
};

} // namespace bembel::particles
