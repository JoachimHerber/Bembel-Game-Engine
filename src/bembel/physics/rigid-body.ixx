module;
#include <bullet/btBulletDynamicsCommon.h>

#include <vector>
#include <memory>
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
    MotionState(Transform t) : m_transform{t} {}

    virtual void getWorldTransform(btTransform& center_of_mass_world_trans) const {
        center_of_mass_world_trans.setOrigin(
            {m_transform->position.x, m_transform->position.y, m_transform->position.z}
        );
        center_of_mass_world_trans.setRotation(
            {m_transform->rotation.x,
             m_transform->rotation.y,
             m_transform->rotation.z,
             m_transform->rotation.w}
        );
    }

    virtual void setWorldTransform(btTransform const& center_of_mass_world_trans) {
        auto pos              = center_of_mass_world_trans.getOrigin();
        auto rot              = center_of_mass_world_trans.getRotation();
        m_transform->position = vec3{pos.x(), pos.y(), pos.z()};
        m_transform->rotation = quat{rot.w(), rot.x(), rot.y(), rot.z()};
    }

  private:
    Transform m_transform;
};

struct RigidBodyData {
    Asset<CollisionShape>        collision_shape;
    std::unique_ptr<btRigidBody> rigid_body;
    std::unique_ptr<MotionState> motion_state;
};

export class RigidBody {
  public:
    class Container : public ComponentContainerBase {
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

      private:
        bool createRigidBody(EntityID entity_id, Asset<CollisionShape> collision_shape, btScalar mass);

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

    void setIsKinematic();

    static constexpr std::string_view COMPONENT_TYPE_NAME = "RigidBody";

  private:
    Container*     m_container;
    RigidBodyData* m_data;
};

} // namespace bembel::physics