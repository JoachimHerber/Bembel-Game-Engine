module;
#include <glbinding/gl/gl.h>
module bembel.graphics.pipeline;

import bembel.base;
import bembel.kernel;
import bembel.graphics.geometry;

namespace bembel::graphics {
using namespace bembel::base;
using namespace bembel::kernel;

GeometryRenderingStage::GeometryRenderingStage(RenderingPipeline& pipline)
  : RenderingPipeline::Stage(pipline) {}

GeometryRenderingStage::~GeometryRenderingStage() {}

bool GeometryRenderingStage::configure(xml::Element const* properties) {
    std::string texture_name;
    if(xml::getAttribute(properties, "DepthOutput", "texture", texture_name))
        setDepthOutputTexture(texture_name);

    for(auto colorOutput : xml::IterateChildElements(properties, "ColorOutput")) {
        unsigned attachment;
        if(xml::getAttribute(colorOutput, "texture", texture_name)
           && xml::getAttribute(colorOutput, "attachment", attachment)) {
            setColorOutputTexture(attachment, texture_name);
        }
    }

    return true;
}

void GeometryRenderingStage::setScene(Scene* scene) {
    m_scene               = scene;
    m_geometry_components = scene ? scene->requestComponentContainer<GeometryComponent>() : nullptr;
    m_position_components = scene ? scene->requestComponentContainer<PositionComponent>() : nullptr;
    m_rotation_components = scene ? scene->requestComponentContainer<RotationComponent>() : nullptr;
}

void GeometryRenderingStage::execute(
    GeometryRenderQueue& renderQueue, std::vector<RendererPtr> const& renderer
) {
    if(!m_scene) return;

    m_fbo->beginRenderToTexture();

    gl::glEnable(gl::GL_CULL_FACE);
    gl::glEnable(gl::GL_DEPTH_TEST);
    gl::glDisable(gl::GL_BLEND);

    gl::glClearColor(0.0, 0.0, 0.0f, 0.0f);
    gl::glClear(gl::GL_COLOR_BUFFER_BIT | gl::GL_DEPTH_BUFFER_BIT);

    auto cam = m_pipline.getCamera();

    auto const& entitis             = m_scene->getEntitys();
    auto const& position_components = m_position_components->getComponentData();
    auto const& rotation_components = m_rotation_components->getComponentData();
    auto const& geometry_components = m_geometry_components->getComponentData();

    renderQueue.clearRendarData();

    for(usize entity = 0; entity < entitis.size(); ++entity) {
        if((entitis[entity] & m_geometry_components->getComponentMask()) == 0) continue;

        mat4 transform(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
        if(entitis[entity] & m_position_components->getComponentMask()) {
            transform = glm::translate(transform, position_components[entity]);
        }
        if(entitis[entity] & m_rotation_components->getComponentMask()) {
            transform = transform * glm::mat4_cast(rotation_components[entity]);
        }

        auto& geom = geometry_components[entity];
        renderQueue.addGeometryObject(geom.model, transform);
    }

    renderQueue.sortRenderData();
    for(auto& it : renderer) {
        it->render(cam->getProjectionMatrix(), cam->getViewMatrix(), renderQueue.getRenderData());
    }
    m_fbo->endRenderToTexture();
}

} // namespace bembel::graphics
