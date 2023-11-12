module;
#include <btBulletDynamicsCommon.h>

#include <memory>
#include <vector>
export module bembel.physics:MotionState;

import bembel.base;
import bembel.kernel;

import :Units;
import :CollisionShape;

namespace bembel::physics {
using namespace bembel::base;
using namespace bembel::kernel;

class MotionState : public btMotionState {
  public:
    MotionState(Transform t, In<vec3> center_of_mass_offset = {0, 0, 0})
      : m_transform{t}, m_center_of_mass_offset{center_of_mass_offset} {}

    virtual void getWorldTransform(btTransform& center_of_mass_world_trans) const {
        quat rot    = m_transform->rotation;
        vec3 origin = m_transform->position + rot * m_center_of_mass_offset;
        center_of_mass_world_trans.setOrigin({origin.x, origin.y, origin.z});
        center_of_mass_world_trans.setRotation({rot.x, rot.y, rot.z, rot.w});
    }

    virtual void setWorldTransform(btTransform const& center_of_mass_world_trans) {
        auto bt_origin = center_of_mass_world_trans.getOrigin();
        auto bt_rot    = center_of_mass_world_trans.getRotation();
        vec3 origin    = {bt_origin.x(), bt_origin.y(), bt_origin.z()};
        quat rot       = {bt_rot.w(), bt_rot.x(), bt_rot.y(), bt_rot.z()};

        auto pos              = origin - rot * m_center_of_mass_offset;
        m_transform->position = pos;
        m_transform->rotation = rot;
    }

    void setCenterOfMassOffset(In<vec3> offset) { m_center_of_mass_offset = offset; }

  private:
    Transform m_transform;
    vec3      m_center_of_mass_offset;
};

} // namespace bembel::physics