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

struct PhysicsComponentData {
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

        EntityID rayTestFirst(In<vec3> ray_start, In<vec3> ray_end);

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
    RigidBody(Container* container = nullptr, EntityID entity, RigidBodyData* data = nullptr)
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
    EntityID       m_entity;
    RigidBodyData* m_data;
};

} // namespace bembel::physics