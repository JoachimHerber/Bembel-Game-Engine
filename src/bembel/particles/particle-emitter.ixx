module;
#include <chrono>
#include <memory>
#include <string>
export module bembel.particles:Emitter;

import bembel.base;
import bembel.kernel;
import :Effects;

namespace bembel::particles {
using namespace bembel::base;
using namespace bembel::kernel;

export struct ParticleEmitter {
    struct Effect {
        Asset<ParticleEffect> effect;
        float                 min_cooldown;
        float                 max_cooldown;
        float                 cooldown;
    };
    std::vector<Effect> effects;
};

export bool initComponent(In<xml::Element const*> properties, Out<ParticleEmitter> component);

} // namespace bembel::particles

export template <>
struct bembel::kernel::ComponentMetaData<bembel::particles::ParticleEmitter>
  : BasicComponentMetaData<
        "ParticleEmitter",
        bembel::particles::ParticleEmitter,
        ComponentContainer::MAP> {};