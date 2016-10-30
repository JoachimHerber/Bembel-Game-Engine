/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "GeometryRenderingStage.h"


#include "RenderingPipeline.h"
#include "Camera.h"

#include <BembelKernel/Rendering/Texture.h>
#include <BembelKernel/Rendering/GeometryRenderer.h>
#include <BembelKernel/Rendering/FrameBufferObject.h>

#include <glm/gtc/matrix_transform.hpp>

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

	glClearColor(0.0, 0.0, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	auto cam = _pipline->GetCamera();

	const auto& entitis   = _scene->GetEntitys();
// 	const auto& posComp = _positionComponents->GetComponents();
// 	const auto& geometrys = _geometryComponents->GetComponents();
	
	std::vector<GeometryRenderer::GeometryInstance> geometry;
	geometry.reserve(entitis.size());
	for (Scene::EntityID entity = 0; entity < entitis.size(); ++entity)
	{
		if ((entitis[entity] & _geometryComponents->GetComponentMask()) == 0)
			continue;

		glm::mat4 transform(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
		if (entitis[entity] & _positionComponents->GetComponentMask())
		{
			transform = glm::translate(
				transform, _positionComponents->GetComponent(entity)->position);
		}

		GeometryComponent* geom = _geometryComponents->GetComponent(entity);
		geometry.push_back({geom->model, transform});
	}

	for ( auto& renderer : _pipline->GetRenderer())
	{
		renderer->DrawGeometry(
			cam->GetViewMatrix(), 
			cam->GetProjectionMatrix(), 
			geometry);
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
