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

    using Container = ComponentMap<bembel::particles::ParticleEmitter>;

    static bool deserialize(Container* container, EntityID entity_id, xml::Element const* entity);
};

} // namespace bembel::particles
