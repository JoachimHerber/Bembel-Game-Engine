module;
#include <btBulletDynamicsCommon.h>

#include <map>
#include <memory>
#include <string_view>
#include <vector>
export module bembel.physics:RigidBody;

import bembel.base;
import bembel.kernel;

import :Units;
import :CollisionShape;
import :World;

namespace bembel::physics {
using namespace bembel::base;
using namespace bembel::kernel;

class MotionState : public btMotionState {
  public:
    MotionState(In<Transform*> t, In<vec3> center_of_mass_offset = {0, 0, 0})
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
    Transform* m_transform;
    vec3       m_center_of_mass_offset;
};

export class RigidBody {
  public:
    RigidBody(
        In<World*>                world,
        In<EntityID>              entity,
        In<Asset<CollisionShape>> collision_shape,
        In<vec3>                  center_of_mass_offset,
        In<units::Kilogram>       mass,
        In<float>                 friction = 0.5f
    );
    ~RigidBody();

    void setCenterOfMassOffset(In<vec3> offset) { m_motion_state.setCenterOfMassOffset(offset); }

    bool makeKinematic();
    bool makeStatic();
    bool makeDynamic(In<units::Kilogram> mass);

    vec3 getLinearVelocity();
    vec3 getAngularVelocity();

    btRigidBody& getRigidBody() { return m_rigid_body; }

    EntityID getEntityID() const { return m_entity; }

  private:
    World*                m_world;
    EntityID              m_entity;
    Asset<CollisionShape> m_collision_shape;
    MotionState           m_motion_state;
    btRigidBody           m_rigid_body;
};

class RigidBodyContainer : public ComponentContainerBase {
  public:
    RigidBodyContainer(ComponentTypeID type_id, Scene* scene);
    ~RigidBodyContainer() {}

    bool assignComponent(
        EntityID                  entity_id,
        In<Asset<CollisionShape>> collision_shape,
        In<vec3>                  center_of_mass_offset,
        In<units::Kilogram>       mass,
        In<float>                 friction = 0.5f
    );

    bool serializeComponent(EntityID, xml::Element*) override { return false; }
    bool deserializeComponent(EntityID, xml::Element const*) override;

    bool deleteComponent(EntityID entity_id) override;

    RigidBody* getComponent(EntityID entity_id);

  private:
    Scene* m_scene;

    std::map<EntityID, std::unique_ptr<RigidBody>> m_rigid_bodys;
};
} // namespace bembel::physics

export template <>
struct bembel::kernel::ComponentMetaData<bembel::physics::RigidBody> {
    static constexpr std::string_view COMPONENT_TYPE_NAME = "RigidBody";
    using Container                                       = bembel::physics::RigidBodyContainer;
};