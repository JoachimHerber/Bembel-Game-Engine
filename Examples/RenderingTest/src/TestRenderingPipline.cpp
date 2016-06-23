/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "TestRenderingPipline.h"

#include <BembelGraphics/OpenGL/ShaderProgram.h>
#include "BembelGraphics/OpenGL/FrameBufferObject.h"
#include "BembelGraphics/RenderingPipeline/Camera.h"
#include "BembelGraphics/OpenGL/Texture.h"

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel {

TestRenderingPipline::TestRenderingPipline()
	: _renderer(std::make_unique<TestRenderer>())
{
	_stages.push_back(std::make_shared<GeometryStage>(this));
	_stages.push_back(std::make_shared<LightingStage>(this));
}

TestRenderingPipline::~TestRenderingPipline()
{}

TestRenderer* TestRenderingPipline::GetRenderer()
{
	return _renderer.get();
}

Texture* TestRenderingPipline::GetNormalBuffer()
{
	return GetTexture(_normalBuffer);
}

Texture* TestRenderingPipline::GetDiffuseBuffer()
{
	return GetTexture(_diffuseBuffer);
}

Texture* TestRenderingPipline::GetSpecularBuffer()
{
	return GetTexture(_specularBuffer);
}

bool TestRenderingPipline::Init()
{
	_normalBuffer   = RequestTexture("NormalBUffer", GL_RGBA);
	_diffuseBuffer  = RequestTexture("DiffuseBUffer", GL_RGBA);
	_specularBuffer = RequestTexture("SpecularBUffer", GL_RGBA);

	return _renderer->Init() && RenderingPipeline::Init();
}

bool TestRenderingPipline::Cleanup()
{
	return _renderer->Cleanup() && RenderingPipeline::Cleanup();
}

TestRenderingPipline::GeometryStage::GeometryStage(TestRenderingPipline* pipline)
	: _pipline(pipline)
{}

TestRenderingPipline::GeometryStage::~GeometryStage()
{}

void TestRenderingPipline::GeometryStage::DoRendering(RenderingPipeline*)
{
	Texture* normal  = _pipline->GetNormalBuffer();
	Texture* diffuse = _pipline->GetDiffuseBuffer();
	Texture* specular = _pipline->GetSpecularBuffer();
	Texture* depth = _pipline->GetDepthBuffer();
	Texture* color = _pipline->GetColorBuffer();

	Camera* cam = _pipline->GetCamera().get();

	if (!depth || !color)
		return;

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,  GL_TEXTURE_2D, depth->GetTextureHandle(), 0);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, diffuse->GetTextureHandle(), 0);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, specular->GetTextureHandle(), 0);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, normal->GetTextureHandle(), 0);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, color->GetTextureHandle(), 0);

	const GLenum drawBuffer[] = {
		GL_COLOR_ATTACHMENT0, 
		GL_COLOR_ATTACHMENT1, 
		GL_COLOR_ATTACHMENT2
	};
	glDrawBuffers(3, drawBuffer);

	glClearColor(0.5, 0.5, 0.5f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_pipline->GetRenderer()->DoGeometryPass(
		cam->GetViewMatrix(), cam->GetProjectionMatrix(), cam->GetViewFrustum());
}

TestRenderingPipline::LightingStage::LightingStage(TestRenderingPipline* pipline)
	: _pipline(pipline)
{}

TestRenderingPipline::LightingStage::~LightingStage()
{}

void TestRenderingPipline::LightingStage::DoRendering(RenderingPipeline*)
{
	Texture* depth = _pipline->GetDepthBuffer();
	Texture* color = _pipline->GetColorBuffer();

	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, 0, 0);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color->GetTextureHandle(), 0);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, 0, 0);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, 0, 0);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);

}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
