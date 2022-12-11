module;
#include <bullet/btBulletDynamicsCommon.h>

#include "bembel/pch.h"
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

    static std::unique_ptr<CollisionShape> createBox(vec3 size);
    static std::unique_ptr<CollisionShape> createSphere(float radius);

    static constexpr std::string_view ASSET_TYPE_NAME = "CollisionShape";

    static std::unique_ptr<CollisionShape> loadAsset(
        AssetManager& asset_mgr, std::filesystem::path file
    );
    static std::unique_ptr<CollisionShape> createAsset(
        AssetManager& asset_mgr, xml::Element const* properties
    );

    static void deleteAsset(AssetManager& asset_mgr, std::unique_ptr<CollisionShape> model);

    using DefaultLoaderType = SerialAssetLoader<CollisionShape>;

    static void initFactory();

  private:
    static std::unique_ptr<CollisionShape> createCollisionShape(
        AssetManager& asset_mgr, xml::Element const* properties
    );

    static Factory<CollisionShape, xml::Element const*>& getFactory();
};

template <typename T>
class TCollisionShape : public CollisionShape {
  public:
    template <typename... TArgs>
    TCollisionShape(TArgs&&... args) : m_shape{std::forward<TArgs>(args)...} {}
    ~TCollisionShape() = default;

    btCollisionShape* getCollisionShape() override { return &m_shape; }
  private:
    T m_shape;
};


} // namespace bembel::physics