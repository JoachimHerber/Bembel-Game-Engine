/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "geometry-rendering-stage.h"

#include "camera.h"
#include "rendering-pipeline.h"
#include "../graphic-system.h"
#include "../geometry/geometry-renderer.h"

#include <glm/gtc/matrix_transform.hpp>

#include <bembel-kernel/rendering/texture.h>
#include <bembel-kernel/rendering/frame-buffer-object.h>

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel {

GeometryRenderingStage::GeometryRenderingStage(RenderingPipeline* pipline)
	: RenderingStage(pipline)
	, _fbo(std::make_unique<FrameBufferObject>())
{}

GeometryRenderingStage::~GeometryRenderingStage()
{}

void GeometryRenderingStage::SetDepthOutputTexture(const std::string& texture)
{
	_fbo->SetDepthAttechment(_pipline->GetTexture(texture));
}

void GeometryRenderingStage::SetColorOutputTexture(
	unsigned index, const std::string& texture)
{
	_fbo->SetColorAttechment(index, _pipline->GetTexture(texture));
}

void GeometryRenderingStage::Init()
{
	_fbo->Init();
}

void GeometryRenderingStage::Cleanup()
{
	_fbo->CleanUp();
}

void GeometryRenderingStage::DoRendering()
{
	if (!_scene)
		return;

 	_fbo->BeginRenderToTexture();

	glEnable(GL_CULL_FACE);
 	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);


	glClearColor(0.0, 0.0, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	auto cam = _pipline->GetCamera();

	const auto& entitis   = _scene->GetEntitys();
// 	const auto& posComp = _positionComponents->GetComponents();
// 	const auto& geometrys = _geometryComponents->GetComponents();
	
	auto& renderQueue = _pipline->GetGraphicSystem()->GetGeometryRenderQueue();
	renderQueue.ClearRendarData();

	for (Scene::EntityID entity = 0; entity < entitis.size(); ++entity)
	{
		if ((entitis[entity] & _geometryComponents->GetComponentMask()) == 0)
			continue;

		glm::mat4 transform(
			1, 0, 0, 0, 
			0, 1, 0, 0, 
			0, 0, 1, 0, 
			0, 0, 0, 1);
		if (entitis[entity] & _positionComponents->GetComponentMask())
		{
			transform = glm::translate(
				transform, _positionComponents->GetComponent(entity)->position);
		}

		GeometryComponent* geom = _geometryComponents->GetComponent(entity);
		renderQueue.AddGeometryObject(geom->model, transform);
	}

	renderQueue.SortRenderData();
	for ( auto& renderer :_pipline->GetGraphicSystem()->GetRenderer())
	{
		renderer->Render(
			cam->GetProjectionMatrix(),
			cam->GetViewMatrix(),
			renderQueue.GetRenderData());
	}
 	_fbo->EndRenderToTexture();
}

void GeometryRenderingStage::SetScene(ScenePtr scene)
{
	_scene = scene;

	_geometryComponents = scene->RequestComponentContainer<GeometryComponent>();
	_positionComponents = scene->RequestComponentContainer<PositionComponent>();
}

std::unique_ptr<GeometryRenderingStage> GeometryRenderingStage::CreateInstance(
	const xml::Element* properties,
	RenderingPipeline* pipline)
{
	auto stage = std::make_unique<GeometryRenderingStage>(pipline);

	std::string textureName;
	if (xml::GetAttribute(properties, "DepthOutput", "texture", textureName))
		stage->SetDepthOutputTexture(textureName);

	for (auto colorOutput : xml::IterateChildElements(properties, "ColorOutput"))
	{
		unsigned attachment;
		if (xml::GetAttribute(colorOutput, "texture", textureName) &&
			xml::GetAttribute(colorOutput, "attachment", attachment))
		{
			stage->SetColorOutputTexture(attachment, textureName);
		}
	}

	return std::move(stage);
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
