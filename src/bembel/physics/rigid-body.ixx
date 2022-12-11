module;
#include <bullet/btBulletDynamicsCommon.h>

#include "bembel/pch.h"
export module bembel.physics:RigidBody;

import bembel.base;
import bembel.kernel;

namespace bembel::physics {
using namespace bembel::base;
using namespace bembel::kernel;

export class RigidBodyComponent {
  public:
    struct RigidBodyData {
        AssetHandle                           collision_shape;
        std::unique_ptr<btRigidBody>          rigid_body;
        std::unique_ptr<btDefaultMotionState> motion_state;
    };

    class Container : public ComponentContainerBase {
      public:
        Container(ComponentTypeID type_id, Scene* scene, btDiscreteDynamicsWorld* world)
          : ComponentContainerBase{type_id}, m_scene{scene}, m_world{world} {}
        virtual ~Container() = default;

        RigidBodyComponent createComponent(EntityID entity_id);

        bool createComponent(EntityID entity_id, xml::Element const* properties) override;

        bool deleteComponent(EntityID entity_id) override;

        auto& getComponentData() { return m_data; }

        RigidBodyComponent getComponent(EntityID entity_id);

      private:
        bool createRigidBody(EntityID entity_id, AssetHandle collision_shape, btScalar mass);

      private:
        Scene*                     m_scene;
        btDiscreteDynamicsWorld*   m_world;
        std::vector<RigidBodyData> m_data;
    };

  public:
    RigidBodyComponent(Container* container, RigidBodyData* data)
      : m_container{container}, m_data{data} {}
    RigidBodyComponent(RigidBodyComponent const&) = default;
    RigidBodyComponent(RigidBodyComponent&&)      = default;

    RigidBodyComponent& operator=(RigidBodyComponent const&) = default;
    RigidBodyComponent& operator=(RigidBodyComponent&&)      = default;

    operator bool() const { return m_container && m_data; }

    static constexpr std::string_view COMPONENT_TYPE_NAME = "RigidBody";

  private:
    Container*     m_container;
    RigidBodyData* m_data;
};

} // namespace bembel::physics