module;
module bembel.physics;

import bembel.base;
import bembel.kernel;

namespace bembel::physics {
using namespace bembel::base;
using namespace bembel::kernel;

void PhysicsSystem::update(double time_since_last_update) {
    for(auto& it : m_worlds) { it->update(time_since_last_update); }
}

} // namespace bembel::physics
