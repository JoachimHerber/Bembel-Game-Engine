module;
#include <btBulletDynamicsCommon.h>
#include <glbinding/gl/gl.h>

#include <filesystem>
#include <memory>
module bembel.physics:DebugRenderer;

import bembel.base;
import bembel.kernel;
import bembel.graphics;

import :RigidBody;
import :World;

namespace bembel::physics {
using namespace bembel::base;
using namespace bembel::kernel;
using namespace bembel::graphics;
using namespace ::gl;

PhysicsDebugRenderStage::PhysicsDebugRenderStage(RenderingPipeline& pipline)
  : RenderingPipeline::Stage{pipline} {}

PhysicsDebugRenderStage::~PhysicsDebugRenderStage() {}

bool PhysicsDebugRenderStage::configure(xml::Element const* properties) {
    std::string texture_name;
    if(xml::getAttribute(properties, "Output", "texture", texture_name)) {
        m_fbo->setColorAttechment(0, m_pipline.getTexture(texture_name));
    }
    if(xml::getAttribute(properties, "Depth", "texture", texture_name)) {
        m_fbo->setDepthAttechment(m_pipline.getTexture(texture_name));
    }
    return true;
}

void PhysicsDebugRenderStage::setScene(Scene* scene) {
    m_scene = scene;
}

void PhysicsDebugRenderStage::init() {
    RenderingPipeline::Stage::init();
}
void PhysicsDebugRenderStage::cleanup() {
    RenderingPipeline::Stage::cleanup();
}
void PhysicsDebugRenderStage::execute(In<std::span<const RendererPtr>>) {
    if(!m_enabled || !m_scene) return;

    auto world = m_scene->getDataContainer<World>()->getWorld();
    if(!world) return;

    auto cam = m_pipline.getCamera();

    m_fbo->beginRenderToTexture();

    mat4 proj = cam->getProjectionMatrix();
    mat4 view = cam->getViewMatrix();

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(glm::value_ptr(proj));
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(glm::value_ptr(view));
    glDisable(GL_CULL_FACE);
    glDisable(GL_BLEND);
    if(m_depth_test) {
        glEnable(GL_DEPTH_TEST);
    } else {
        glDisable(GL_DEPTH_TEST);
    }

    glUseProgram(0);
    glLineWidth(1);

    world->setDebugDrawer(&m_debug_draw);
    world->debugDrawWorld();
    world->setDebugDrawer(nullptr);

    m_fbo->endRenderToTexture();
}

void PhysicsDebugRenderStage::handleEvent(In<ConfigurePhysicsDebugRenderStageEvent> event) {
    switch(event.enable) {
        case ConfigurePhysicsDebugRenderStageEvent::FALSE: m_enabled = false; break;
        case ConfigurePhysicsDebugRenderStageEvent::TRUE: m_enabled = true; break;
        case ConfigurePhysicsDebugRenderStageEvent::TOGGLE: m_enabled = !m_enabled; break;
    }
    switch(event.depth_test) {
        case ConfigurePhysicsDebugRenderStageEvent::FALSE: m_depth_test = false; break;
        case ConfigurePhysicsDebugRenderStageEvent::TRUE: m_depth_test = true; break;
        case ConfigurePhysicsDebugRenderStageEvent::TOGGLE: m_depth_test = !m_depth_test; break;
    }
}

void PhysicsDebugRenderStage::DebugDraw::drawLine(
    const btVector3& from, const btVector3& to, const btVector3& color
) {
    glBegin(GL_LINES);
    glColor3f(color.getX(), color.getY(), color.getZ());
    glVertex3f(from.getX(), from.getY(), from.getZ());
    glVertex3f(to.getX(), to.getY(), to.getZ());
    glEnd();
}

} // namespace bembel::physics