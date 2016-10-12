/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "DeferredGeometryStage.h"


#include "RenderingPipeline.h"
#include "Camera.h"
#include "Renderer.h"

#include <BembelKernel/Renderig/Texture.h>
#include <BembelKernel/Renderig/FrameBufferObject.h>

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel {

DeferredGeometryStage::DeferredGeometryStage(RenderingPipeline* pipline)
	: RenderingStage(pipline)
	, _fbo(std::make_shared<FrameBufferObject>())
{}

DeferredGeometryStage::~DeferredGeometryStage()
{}

void DeferredGeometryStage::SetDepthOutputTexture(const std::string& texture)
{
	_fbo->SetDepthAttechment(_pipline->GetTexture(texture));
}

void DeferredGeometryStage::SetColorOutputTexture(unsigned index, const std::string& texture)
{
	_fbo->SetColorAttechment(index, _pipline->GetTexture(texture));
}

void DeferredGeometryStage::Init()
{
	_fbo->Init();
}

void DeferredGeometryStage::Cleanup()
{
	_fbo->CleanUp();
}

void DeferredGeometryStage::DoRendering()
{
 	_fbo->BeginRenderToTexture();

	glEnable(GL_CULL_FACE);
 	glEnable(GL_DEPTH_TEST);

	glClearColor(0.0, 0.0, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	auto cam = _pipline->GetCamera();

	for ( auto& renderer : _pipline->GetRenderer())
	{
		renderer->DoGeometryPass(
			cam->GetViewMatrix(), 
			cam->GetProjectionMatrix(), 
			cam->GetViewFrustum());
	}
 	_fbo->EndRenderToTexture();
}

std::unique_ptr<DeferredGeometryStage> DeferredGeometryStage::CreateInstance(
	const xml::Element* properties,
	RenderingPipeline* pipline)
{
	auto stage = std::make_unique<DeferredGeometryStage>(pipline);

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
