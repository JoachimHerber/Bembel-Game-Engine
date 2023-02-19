module;
#include <glbinding/gl/gl.h>

#include <cstdlib>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
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
        m_scene->registerComponentType<Geometry>();
        m_scene->registerComponentType<Transform>();

        m_geometrys  = scene->getComponentContainer<Geometry>();
        m_transforms = scene->getComponentContainer<Transform>();
    } else {
        m_geometrys  = nullptr;
        m_transforms = nullptr;
    }
}

void GeometryRenderingStage::execute(
    GeometryRenderQueue& render_queue, std::vector<RendererPtr> const& renderer
) {
    if(!m_scene) return;

    m_fbo->beginRenderToTexture();

    gl::glEnable(gl::GL_CULL_FACE);
    gl::glEnable(gl::GL_DEPTH_TEST);
    gl::glDisable(gl::GL_BLEND);

    gl::glClearColor(0.0, 0.0, 0.0f, 0.0f);
    gl::glClear(gl::GL_COLOR_BUFFER_BIT | gl::GL_DEPTH_BUFFER_BIT);

    auto cam = m_pipline.getCamera();

    auto const& entities  = m_scene->getEntitys();
    auto const& geometrys = m_geometrys->getComponentData();
    auto        transform = m_transforms->begin();

    render_queue.clearRendarData();

    for(usize entity = 0; entity < entities.size(); ++entity, ++transform) {
        if((entities[entity] & m_geometrys->getComponentMask()) == 0) continue;

        auto& geom = geometrys[entity];

        mat4 model_matrix = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
        if(entities[entity] & m_transforms->getComponentMask()) {
            model_matrix = glm::translate(model_matrix, transform->position);
            model_matrix = model_matrix * glm::mat4_cast(transform->rotation);
            model_matrix = glm::scale(model_matrix, vec3(transform->scale));
        }
        model_matrix = glm::scale(model_matrix, geom.scale);

        render_queue.addGeometryObject(geom.model.get(), model_matrix);
    }

    render_queue.sortRenderData();
    for(auto& it : renderer) {
        it->renderGeometry(
            cam->getProjectionMatrix(), cam->getViewMatrix(), render_queue.getRenderData()
        );
    }
    m_fbo->endRenderToTexture();
}

} // namespace bembel::graphics
