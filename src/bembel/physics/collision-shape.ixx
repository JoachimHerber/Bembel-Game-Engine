module;
#include <btBulletDynamicsCommon.h>

#include <array>
#include <filesystem>
#include <memory>
#include <vector>
export module bembel.physics:CollisionShape;

import bembel.base;
import bembel.kernel;

namespace bembel::physics {
using namespace bembel::base;
using namespace bembel::kernel;

export class CollisionShape {
  protected:
    CollisionShape() = default;

  public:
    virtual ~CollisionShape() = default;

    virtual btCollisionShape* getCollisionShape() = 0;

    static constexpr std::string_view ASSET_TYPE_NAME = "CollisionShape";

    static std::unique_ptr<CollisionShape> loadAsset(std::filesystem::path file);
    static std::unique_ptr<CollisionShape> createAsset(xml::Element const* properties);

    static void deleteAsset(std::unique_ptr<CollisionShape> model);

    using DefaultLoaderType = SerialAssetLoader<CollisionShape>;

    static void initFactory();

  private:
    static std::unique_ptr<CollisionShape> createCollisionShape(xml::Element const* properties);

    static Factory<CollisionShape, xml::Element const*>& getFactory();
};

export template <typename T>
class TCollisionShape : public CollisionShape {
  public:
    template <typename... TArgs>
    TCollisionShape(TArgs&&... args) : m_shape{std::forward<TArgs>(args)...} {
        m_shape.setUserPointer(this);
    }
    ~TCollisionShape() = default;

    btCollisionShape* getCollisionShape() override { return &m_shape; }

  private:
    T m_shape;
};

export class CollisionCompoundShape : public CollisionShape {
  public:
    CollisionCompoundShape() : m_shape{} { m_shape.setUserPointer(this); }
    ~CollisionCompoundShape() = default;

    void addChildShape(Move<std::unique_ptr<CollisionShape>> shape, In<vec3> pos, In<quat> rot) {
        m_child_shapes.push_back(std::move(shape));
        m_shape.addChildShape(
            btTransform{btQuaternion{rot.x, rot.y, rot.z, rot.w}, btVector3{pos.x, pos.y, pos.z}},
            m_child_shapes.back()->getCollisionShape()
        );
    }

    btCollisionShape* getCollisionShape() override { return &m_shape; }

  protected:
    btCompoundShape                              m_shape;
    std::vector<std::unique_ptr<CollisionShape>> m_child_shapes;
};

export using CollisionBox      = TCollisionShape<btBoxShape>;
export using CollisionSphere   = TCollisionShape<btSphereShape>;
export using CollisionCylinder = TCollisionShape<btCylinderShape>;
export using CollisionCone     = TCollisionShape<btConeShape>;

} // namespace bembel::physics