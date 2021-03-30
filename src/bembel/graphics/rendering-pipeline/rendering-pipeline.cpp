#include "./rendering-pipeline.hpp"

#include <bembel/kernel/core/kernel.hpp>
#include <bembel/kernel/display/display-manager.hpp>
#include <bembel/kernel/display/viewport.hpp>
#include <bembel/kernel/display/window.hpp>
#include <bembel/kernel/rendering/frame-buffer-object.hpp>
#include <bembel/kernel/rendering/shader.hpp>
#include <bembel/kernel/rendering/texture.hpp>

#include "../geometry/renderer.hpp"
#include "../graphic-system.hpp"
#include "../texture-view.hpp"
#include "./camera.hpp"
#include "./rendering-stage.hpp"

namespace bembel::graphics {

RenderingPipeline::RenderingPipeline(GraphicSystem& grapic_system)
: grapic_system(grapic_system)
, resolution(800, 600)
, enabled(true)
, initalized(false)
, scene(nullptr)
, camera(std::make_shared<Camera>()) {
}

RenderingPipeline::~RenderingPipeline() {
}

GraphicSystem& RenderingPipeline::getGraphicSystem() {
    return grapic_system;
}

kernel::AssetManager& RenderingPipeline::getAssetManager() {
    return grapic_system.getAssetManager();
}

void RenderingPipeline::enable() {
    enabled = true;
}
void RenderingPipeline::disable() {
    enabled = false;
}
bool RenderingPipeline::isEnabled() {
    return enabled;
}

void RenderingPipeline::setResulution(const glm::ivec2& value) {
    if(this->resolution == value) return;

    this->resolution = value;
    if(this->initalized) {
        for(auto& it : this->textures) it.second->setSize(value);
    }
}

const glm::ivec2& RenderingPipeline::getResulution() const {
    return this->resolution;
}

bool RenderingPipeline::configure(const base::xml::Element* properties) {
    if(!properties) return false;

    base::xml::getAttribute(properties, "horizontal_resolution", this->resolution.x);
    base::xml::getAttribute(properties, "vertical_resolution", this->resolution.y);

    this->configureTextures(properties->FirstChildElement("Textures"));
    this->configureStages(properties->FirstChildElement("RenderingStages"));
    this->configureViews(properties->FirstChildElement("Views"));
    this->configureCamera(properties->FirstChildElement("Camera"));
    return true;
}

void RenderingPipeline::init() {
    for(auto& it : this->textures) it.second->init(this->resolution);

    for(auto& stage : this->render_stages) stage->init();

    for(auto& view : this->views) view->init();

    this->initalized = true;
}

void RenderingPipeline::cleanup() {
    for(auto& view : this->views) view->cleanup();

    for(auto& stage : this->render_stages) stage->cleanup();

    for(auto& it : this->textures) it.second->cleanup();

    this->initalized = false;
}

void RenderingPipeline::setScene(std::shared_ptr<kernel::Scene> scene) {
    this->scene = scene;

    for(auto& stage : this->render_stages) stage->setScene(this->scene.get());
}

std::shared_ptr<kernel::Scene> RenderingPipeline::getScene() const {
    return this->scene;
}

std::shared_ptr<Camera> RenderingPipeline::getCamera() const {
    return this->camera;
}

kernel::Texture* RenderingPipeline::getTexture(const std::string& name) const {
    auto it = this->textures.find(name);
    if(it != this->textures.end()) return it->second.get();

    return nullptr;
}

bool RenderingPipeline::createTexture(const std::string& name, GLenum format) {
    if(this->textures.find(name) != this->textures.end()) return false; // texture already exists

    auto texture = std::make_unique<kernel::Texture>(GL_TEXTURE_2D, format);
    if(this->initalized) texture->init(this->resolution);

    this->textures.emplace(name, std::move(texture));
    return true;
}

bool RenderingPipeline::createTexture(const base::xml::Element* properties) {
    std::string name, format, target;
    if(!base::xml::getAttribute(properties, "name", name)) return false;

    base::xml::getAttribute(properties, "format", format);

    return this->createTexture(name, this->stringToTextureFormat(format));
}

TextureView* RenderingPipeline::createView(const std::string& texture_name) {
    kernel::Texture* texture = getTexture(texture_name);
    if(!texture) return nullptr;

    this->views.push_back(std::make_unique<TextureView>(texture));
    if(this->initalized) this->views.back()->init();

    return this->views.back().get();
}

void RenderingPipeline::update() {
    glViewport(0, 0, this->resolution.x, this->resolution.y);
    for(auto& stage : this->render_stages) stage->execute();
}

const std::vector<std::unique_ptr<TextureView>>& RenderingPipeline::getViews() const {
    return this->views;
}

void RenderingPipeline::configureTextures(const base::xml::Element* properties) {
    if(!properties) return;

    for(auto textureProperties : base::xml::IterateChildElements(properties, "Texture")) {
        this->createTexture(textureProperties);
    }
}

void RenderingPipeline::configureStages(const base::xml::Element* properties) {
    if(!properties) return;

    for(auto stageProperties : base::xml::IterateChildElements(properties)) {
        auto stage = this->grapic_system.getRendertingStageFactory().createObject(
            stageProperties->Value(), stageProperties, *this);

        if(stage) this->render_stages.push_back(std::move(stage));
    }
}

void RenderingPipeline::configureViews(const base::xml::Element* properties) {
    if(!properties) return;

    for(auto viewProperties : base::xml::IterateChildElements(properties, "View")) {
        std::string texture;
        base::xml::getAttribute(viewProperties, "texture", texture);

        auto  view = this->createView(texture);
        float l = 0, r = 1, b = 0, t = 1;
        base::xml::getAttribute(viewProperties, "left", l);
        base::xml::getAttribute(viewProperties, "right", r);
        base::xml::getAttribute(viewProperties, "bottom", b);
        base::xml::getAttribute(viewProperties, "top", t);
        view->setViewArea(
            glm::vec2(this->resolution) * glm::vec2(l, b),
            glm::vec2(this->resolution) * glm::vec2(r - l, t - b));

        unsigned windowId, viewportId;
        if(base::xml::getAttribute(viewProperties, "window", windowId)
           && base::xml::getAttribute(viewProperties, "viewport", viewportId)) {
            auto& display_mgr = this->grapic_system.getKernel().getDisplayManager();
            auto  window      = display_mgr.getWindow(windowId);

            if(window && window->getViewports().size() > viewportId) {
                window->getViewports()[viewportId]->addView(view);
            }
        }
    }
}

void RenderingPipeline::configureCamera(const base::xml::Element* properties) {
    if(!properties) return;

    glm::vec3 pos;
    base::xml::getAttribute(properties, "Position", "x", pos.x);
    base::xml::getAttribute(properties, "Position", "y", pos.y);
    base::xml::getAttribute(properties, "Position", "z", pos.z);
    this->camera->setPosition(pos);

    float fov = 60, ar = (4 / 3), near = 0.1f, far = 1000.0f;
    base::xml::getAttribute(properties, "Projection", "fild_of_view", fov);
    base::xml::getAttribute(properties, "Projection", "aspect_ration", ar);
    base::xml::getAttribute(properties, "Projection", "near", near);
    base::xml::getAttribute(properties, "Projection", "far", far);
    fov *= 3.14159265359f / 180.0f;
    this->camera->setUpProjection(fov, ar, near, far);
}

GLenum RenderingPipeline::stringToTextureFormat(const std::string& format) {
    const static std::map<std::string, GLenum> mapping{
        {"GL_DEPTH_COMPONENT32", GL_DEPTH_COMPONENT32},
        {"GL_DEPTH_COMPONENT24", GL_DEPTH_COMPONENT24},
        {"GL_DEPTH_COMPONENT16", GL_DEPTH_COMPONENT16},
        {"GL_DEPTH_COMPONENT", GL_DEPTH_COMPONENT},
        {"GL_RGBA", GL_RGBA},
        {"GL_RGBA8", GL_RGBA8},
        {"GL_RGBA16", GL_RGBA16},
        {"GL_RGBA16F", GL_RGBA16F},
        {"GL_RGBA32F", GL_RGBA32F},
        {"GL_RGB", GL_RGB},
        {"GL_RGB8", GL_RGB8},
        {"GL_RGB16", GL_RGB16},
        {"GL_RGB16F", GL_RGB16F},
        {"GL_RGB32F", GL_RGB32F},

        {"GL_SRGB8", GL_SRGB8},
        {"GL_SRGB8_ALPHA8", GL_SRGB8_ALPHA8},
    };
    auto it = mapping.find(format);
    if(it != mapping.end()) return it->second;

    return GL_RGBA;
}

} // namespace bembel::graphics
