module;
module bembel.particles;

import bembel.base;
import bembel.kernel;

namespace bembel::particles {
using namespace bembel::base;
using namespace bembel::kernel;

bool ParticleEmitter::deserialize(
    Container* container, EntityID entity_id, xml::Element const* entity
) {
    return true;
}

} // namespace bembel::particles
