module;
module bembel.particles;

import bembel.base;
import bembel.kernel;

namespace bembel::particles {
using namespace bembel::base;
using namespace bembel::kernel;

bool initComponent(In<xml::Element const*> properties, Out<ParticleEmitter> component){
  return true;
}

} // namespace bembel::particles
