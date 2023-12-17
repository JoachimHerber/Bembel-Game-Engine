module;
#include <glbinding/gl/gl.h>

#include <format>
#include <memory>
#include <string>
module bembel.examples.rendering;

namespace bembel {
using namespace bembel::graphics;
using namespace bembel::base;
using namespace bembel::kernel;
using namespace bembel::kernel::i18n::literals;
using namespace bembel::gui;
using namespace ::gl;

RenderingExample::RenderingExample() {
    events::addHandler<WindowShouldCloseEvent>(this);
    events::addHandler<FrameBufferResizeEvent>(this);
}

RenderingExample::~RenderingExample() {
    events::removeHandler<WindowShouldCloseEvent>(this);
    events::removeHandler<FrameBufferResizeEvent>(this);
}

bool RenderingExample::init(std::span<std::string_view> args) {
    if(!m_engine.loadSetting("rendering/config.xml")) return false;

    kernel::i18n::Localisation::init(args, "local");

    m_camera = std::make_unique<CameraControle>(
        getSystem<GraphicSystem>()->getRenderingPipelines()[0]->getCamera()
    );

    auto& rendering_piupeline = getSystem<GraphicSystem>()->getRenderingPipelines()[0];
    auto* lighting_stage      = rendering_piupeline->getRenderingStage<DeferredLightingStage>(1);
    if(!lighting_stage) return false;

    m_scene = std::make_shared<kernel::Scene>();
    rendering_piupeline->setScene(m_scene);
    m_scene->loadScene("scenes/material-test.scene");

    lighting_stage->setDirLightShadowResolution(1024);
    auto& shadow_map = lighting_stage->getDirectionalLightShadowMap();

    m_views[0] = std::make_unique<ShadowDebugView>(&shadow_map.getTexture(), 1024, 0);
    m_views[1] = std::make_unique<ShadowDebugView>(&shadow_map.getTexture(), 1024, 1);
    m_views[2] = std::make_unique<ShadowDebugView>(&shadow_map.getTexture(), 1024, 2);

    m_light = Entity(*m_scene);
    m_light.assign<DirectionalLight>(vec3(3.f, 3.f, 3.f), vec3(0.f, -1.f, 0.f), true);

    m_engine.display.getWindow(0)->getViewport(4)->addView(m_views[0].get());
    m_engine.display.getWindow(0)->getViewport(5)->addView(m_views[1].get());
    m_engine.display.getWindow(0)->getViewport(6)->addView(m_views[2].get());

    auto gui = getSystem<GuiSystem>()->getGUI("main");

    m_label = gui->getWidget<LabelWidget>("Label");

    m_light_slider_pitch = gui->getWidget<IntSliderWidget>("Pitch");
    m_light_slider_yaw   = gui->getWidget<IntSliderWidget>("Yaw");

    m_light_slider_pitch->value_update_signal.bind(this, &RenderingExample::updateLightDir);
    m_light_slider_yaw->value_update_signal.bind(this, &RenderingExample::updateLightDir);

    m_engine.initSystems();
    return true;
}

void RenderingExample::cleanup() {
    m_camera.reset();
    m_scene.reset();
    m_engine.shutdownSystems();
    assets::deleteUnusedAssets();
    m_engine.display.closeOpenWindows();
}

void RenderingExample::update(double time) {
    // m_camera->update(time);

    static constexpr float RAD_TO_DEG = 180 / 3.14159265359;

    static bembel::kernel::i18n::String<float, float, float, float, float> text = {
        "examples.rendering.camara_transform_label"_i18n};

    m_label->setText(text(
        m_camera->getPosition().x,
        m_camera->getPosition().y,
        m_camera->getPosition().z,
        int(RAD_TO_DEG * m_camera->getPitch()),
        int(RAD_TO_DEG * m_camera->getYaw())
    ));
}

void RenderingExample::handleEvent(In<WindowShouldCloseEvent> event) {
    quit();
}

void RenderingExample::handleEvent(In<FrameBufferResizeEvent> event) {}

void RenderingExample::updateLightDir(In<i64>) {
    static constexpr float DEG_TO_RAD = 3.14159265359 / 180;

    float pitch = DEG_TO_RAD * m_light_slider_pitch->getValue();
    float yaw   = DEG_TO_RAD * m_light_slider_yaw->getValue();

    auto light = m_light.get<DirectionalLight>();

    light->direction = vec3(   //
        cos(pitch) * cos(yaw), //
        sin(pitch),
        cos(pitch) * sin(yaw)
    );
}

RenderingExample::ShadowDebugView::ShadowDebugView(Texture* texture, u64 resolution, uint layer)
  : m_texture{texture}, m_resolution{resolution}, m_layer{layer} {
    Asset<Shader> vert;
    if(!vert.request("shadow-debug-view.vert")) return;

    Asset<Shader> frag;
    if(!frag.request("shadow-debug-view.frag")) return;

    m_shader = std::make_unique<ShaderProgram>();
    m_shader->attachShader(std::move(vert));
    m_shader->attachShader(std::move(frag));
    if(!m_shader->link()) { m_shader.reset(); }
}
void RenderingExample::ShadowDebugView::draw(ivec2 const& viewport_position, uvec2 const& viewport_size) {
    if(!m_shader) return;
    glViewport(viewport_position.x, viewport_position.y, viewport_size.x, viewport_size.y);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glDisable(GL_CULL_FACE);
    glDisable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_ALPHA_TEST);

    m_shader->use();
    m_shader->setUniform("uLayer", int(m_layer));
    glActiveTexture(GL_TEXTURE0);
    m_texture->bind();

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    m_texture->release();
}
} // namespace bembel
