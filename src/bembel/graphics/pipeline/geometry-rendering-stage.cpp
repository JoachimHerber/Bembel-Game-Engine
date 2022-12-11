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
    m_scene = scene;
    if(scene) {
        m_scene->registerComponentType<GeometryComponent>();
        m_scene->registerComponentType<PositionComponent>();
        m_scene->registerComponentType<RotationComponent>();

        m_geometry_components = scene->getComponentContainer<GeometryComponent>();
        m_position_components = scene->getComponentContainer<PositionComponent>();
        m_rotation_components = scene->getComponentContainer<RotationComponent>();
    } else {
        m_geometry_components = nullptr;
        m_position_components = nullptr;
        m_rotation_components = nullptr;
    }
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

    auto const& entities            = m_scene->getEntitys();
    auto const& position_components = m_position_components->getComponentData();
    auto const& rotation_components = m_rotation_components->getComponentData();
    auto const& geometry_components = m_geometry_components->getComponentData();

    renderQueue.clearRendarData();

    for(usize entity = 0; entity < entities.size(); ++entity) {
        if((entities[entity] & m_geometry_components->getComponentMask()) == 0) continue;

        auto& geom = geometry_components[entity];
        // clang-format off
        mat4 transform = {
            1.0f, 0.0f, 0.0f, 0.0f, 
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };
        // clang-format on
        if(entities[entity] & m_position_components->getComponentMask()) {
            transform = glm::translate(transform, position_components[entity]);
        }
        transform = glm::scale(transform, geom.scale);
        if(entities[entity] & m_rotation_components->getComponentMask()) {
            transform = transform * glm::mat4_cast(rotation_components[entity]);
        }

        renderQueue.addGeometryObject(geom.model, transform);
    }

    renderQueue.sortRenderData();
    for(auto& it : renderer) {
        it->render(cam->getProjectionMatrix(), cam->getViewMatrix(), renderQueue.getRenderData());
    }
    m_fbo->endRenderToTexture();
}

} // namespace bembel::graphics
