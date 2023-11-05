module;
#include <btBulletDynamicsCommon.h>

#include <memory>
#include <vector>
export module bembel.physics:RigidBody;

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

struct RigidBodyData {
    Asset<CollisionShape>        collision_shape;
    std::unique_ptr<btRigidBody> rigid_body;
    std::unique_ptr<MotionState> motion_state;
};

export class RigidBody {
  public:
    class Container : public ComponentContainerBase {
        friend RigidBody;

      public:
        Container(ComponentTypeID type_id, Scene* scene, btDiscreteDynamicsWorld* world)
          : ComponentContainerBase{type_id}, m_scene{scene}, m_world{world} {}
        virtual ~Container() = default;

        RigidBody createComponent(EntityID entity_id);

        RigidBody createComponent(
            EntityID entity_id, Asset<CollisionShape> collision_shape, units::Kilogram mass
        );

        bool createComponent(EntityID entity_id, xml::Element const* properties) override;

        bool deleteComponent(EntityID entity_id) override;

        auto& getComponentData() { return m_data; }

        RigidBody getComponent(EntityID entity_id);

        btDiscreteDynamicsWorld* getWorld() { return m_world; }

      private:
        bool createRigidBody(
            EntityID entity_id, Asset<CollisionShape> collision_shape, btScalar mass
        );

      private:
        Scene*                     m_scene;
        btDiscreteDynamicsWorld*   m_world;
        std::vector<RigidBodyData> m_data;
    };

  public:
    RigidBody(Container* container = nullptr, RigidBodyData* data = nullptr)
      : m_container{container}, m_data{data} {}
    RigidBody(RigidBody const&) = default;
    RigidBody(RigidBody&&)      = default;

    RigidBody& operator=(RigidBody const&) = default;
    RigidBody& operator=(RigidBody&&)      = default;

    operator bool() const { return m_container && m_data; }

    bool init(
        In<Asset<CollisionShape>> collision_shape,
        In<units::Kilogram>       mass,
        In<vec3>                  center_of_mass_offset = {0, 0, 0}
    );

    void setIsKinematic();

    static constexpr std::string_view COMPONENT_TYPE_NAME = "RigidBody";

  private:
    Container*     m_container;
    RigidBodyData* m_data;
};

} // namespace bembel::physics