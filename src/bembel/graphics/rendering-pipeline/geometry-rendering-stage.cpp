#include "./geometry-rendering-stage.hpp"

#include "../geometry/renderer.hpp"
#include "../graphic-system.hpp"
#include "./camera.hpp"
#include "./rendering-pipeline.hpp"

namespace bembel::graphics {

GeometryRenderingStage::GeometryRenderingStage(RenderingPipeline& pipline)
: RenderingStage(pipline) {
}

GeometryRenderingStage::~GeometryRenderingStage() {
}

void GeometryRenderingStage::execute() {
    if(!this->scene) return;

    this->fbo->beginRenderToTexture();

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    glClearColor(0.0, 0.0, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto cam = this->pipline.getCamera();

    const auto& entitis             = this->scene->getEntitys();
    const auto& position_components = this->position_components->getComponents();
    const auto& rotation_components = this->rotation_components->getComponents();
    const auto& geometry_components = this->geometry_components->getComponents();

    auto& renderQueue = this->pipline.getGraphicSystem().getGeometryRenderQueue();
    renderQueue.clearRendarData();

    for(kernel::Scene::EntityID entity = 0; entity < entitis.size(); ++entity) {
        if((entitis[entity] & this->geometry_components->getComponentMask()) == 0) continue;

        glm::mat4 transform(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
        if(entitis[entity] & this->position_components->getComponentMask()) {
            transform = glm::translate(transform, position_components[entity].position);
        }
        if(entitis[entity] & this->rotation_components->getComponentMask()) {
            transform = transform * glm::mat4_cast(rotation_components[entity].rotation);
        }

        const GeometryComponent& geom = geometry_components[entity];
        renderQueue.addGeometryObject(geom.model, transform);
    }

    renderQueue.sortRenderData();
    for(auto& renderer : this->pipline.getGraphicSystem().getRenderer()) {
        renderer->render(
            cam->getProjectionMatrix(), cam->getViewMatrix(), renderQueue.getRenderData());
    }
    this->fbo->endRenderToTexture();
}

void GeometryRenderingStage::setScene(kernel::Scene* scene) {
    this->scene = scene;

    this->geometry_components = scene->requestComponentContainer<GeometryComponent>();
    this->position_components = scene->requestComponentContainer<kernel::PositionComponent>();
    this->rotation_components = scene->requestComponentContainer<kernel::RotationComponent>();
}

std::unique_ptr<GeometryRenderingStage> GeometryRenderingStage::createInstance(
    const base::xml::Element* properties, RenderingPipeline& pipline) {
    auto stage = std::make_unique<GeometryRenderingStage>(pipline);

    std::string texture_name;
    if(base::xml::getAttribute(properties, "DepthOutput", "texture", texture_name))
        stage->setDepthOutputTexture(texture_name);

    for(auto colorOutput : base::xml::IterateChildElements(properties, "ColorOutput")) {
        unsigned attachment;
        if(base::xml::getAttribute(colorOutput, "texture", texture_name)
           && base::xml::getAttribute(colorOutput, "attachment", attachment)) {
            stage->setColorOutputTexture(attachment, texture_name);
        }
    }

    return std::move(stage);
}

} // namespace bembel::graphics
