module;
#include <btBulletDynamicsCommon.h>
// #include <btIDebugDraw.h>

#include <filesystem>
#include <memory>
#include <span>
export module bembel.physics:DebugRenderer;

import bembel.base;
import bembel.kernel;
import bembel.graphics;

namespace bembel::physics {
using namespace bembel::base;
using namespace bembel::kernel;
using namespace bembel::graphics;

export struct ConfigurePhysicsDebugRenderStageEvent {
    enum Config : u8 { FALSE, TRUE, TOGGLE, NO_CHANGE };

    Config enable     = NO_CHANGE;
    Config depth_test = NO_CHANGE;
};

export class PhysicsDebugRenderStage : public RenderingPipeline::Stage {
  public:
    PhysicsDebugRenderStage(RenderingPipeline& pipline);
    ~PhysicsDebugRenderStage();

    void enable() { m_enabled = true; }
    void disable() { m_enabled = false; }

    virtual bool configure(xml::Element const*) override;

    virtual void setScene(Scene*) override;

    virtual void init() override;
    virtual void cleanup() override;
    virtual void execute(In<std::span<const RendererPtr>> renderer) override;

    void handleEvent(In<ConfigurePhysicsDebugRenderStageEvent> event);

  private:
    EventHandlerGuard<ConfigurePhysicsDebugRenderStageEvent> m_guard{this};

    Scene* m_scene      = nullptr;
    bool   m_enabled    = false;
    bool   m_depth_test = false;

    class DebugDraw : public btIDebugDraw {
      public:
        virtual void drawLine(btVector3 const& from, btVector3 const& to, btVector3 const& color)
            override;
        virtual void drawContactPoint(
            const btVector3& PointOnB,
            const btVector3& normalOnB,
            btScalar         distance,
            int              lifeTime,
            const btVector3& color
        ) override {}

        virtual void reportErrorWarning(const char* warning_string) override {
            // log().error("{}", warningString);
        }

        virtual void draw3dText(const btVector3& location, const char* text_string) override {}

        virtual void setDebugMode(int debug_mode) override { m_debug_mode = debug_mode; }

        virtual int getDebugMode() const override { return m_debug_mode; }

      private:
        int m_debug_mode = btIDebugDraw::DBG_DrawWireframe;
    };
    DebugDraw m_debug_draw;
};

} // namespace bembel::physics