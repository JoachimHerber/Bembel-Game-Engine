/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "RenderingPipeline.h"
#include "RenderingStage.h"
#include "Camera.h"

#include "../TextureView.h"
#include "../GraphicSystem.h"

#include <BembelKernel/Renderig/Texture.h>
#include <BembelKernel/Renderig/Shader.h>
#include <BembelKernel/Renderig/FrameBufferObject.h>

/*============================================================================*/
/* IMPLEMENTATION			                                                  */
/*============================================================================*/
namespace bembel{

RenderingPipeline::RenderingPipeline(GraphicSystem* graphicSys)
	: _grapicSys(graphicSys)
	, _resolution(800,600)
	, _enabled(true)
	, _initalized(false)
	, _camera(std::make_shared<Camera>())
{}

RenderingPipeline::~RenderingPipeline()
{}

void RenderingPipeline::Enable()
{
	_enabled = true;
}
void RenderingPipeline::Disable()
{
	_enabled = false;
}
bool RenderingPipeline::IsEnabled()
{
	return _enabled;
}

void RenderingPipeline::SetResulution(const glm::ivec2& value)
{
	if (_resolution == value)
		return;

	_resolution = value;
	if (_initalized)
	{
		for (auto& it : _textures)
			it.second->SetSize(value);
	}
}

const glm::ivec2& RenderingPipeline::GetResulution() const
{
	return _resolution;
}

void RenderingPipeline::Init()
{
	for (auto& it : _textures)
		it.second->Init(_resolution);

	for (auto& stage : _stages)
		stage->Init();

	for (auto& renderer : _renderer)
		renderer->Init();

	for (auto& view : _views)
		view->Init();
}

bool RenderingPipeline::Configure(const xml::Element* properties)
{
	if (!properties)
		return false;

	xml::GetAttribute(properties, "horizontal_resolution", _resolution.x);
	xml::GetAttribute(properties, "vertical_resolution", _resolution.y);

	InitTextures(properties->FirstChildElement("Textures"));
	InitStages(properties->FirstChildElement("RenderingStages"));
	InitRenderer(properties->FirstChildElement("Renderer"));
	InitViews(properties->FirstChildElement("Views"));
	InitCamera(properties->FirstChildElement("Camera"));
	return true;
}

void RenderingPipeline::Cleanup()
{
	for (auto& view : _views)
		view->Cleanup();

	for (auto& renderer : _renderer)
		renderer->Cleanup();

	for (auto& stage : _stages)
		stage->Cleanup();

	for (auto& it : _textures)
		it.second->Cleanup();
}

void RenderingPipeline::SetScene(ScenePtr scene)
{
	_scene = scene;

	for (auto& renderer : _renderer)
		renderer->SetScene(_scene);

	for (auto& stage : _stages)
		stage->SetScene(_scene);
}

std::shared_ptr<Scene> RenderingPipeline::GetScene() const
{
	return _scene;
}

std::shared_ptr<Camera> RenderingPipeline::GetCamera() const
{
	return _camera;
}

std::shared_ptr<Texture> RenderingPipeline::GetTexture(
	const std::string& name) const
{
	auto it = _textures.find(name);
	if (it != _textures.end())
		return it->second;

	return nullptr;
}
std::shared_ptr<Texture> RenderingPipeline::CreateTexture(
	const std::string& name, 
	GLenum format)
{
	if (_textures.find(name) != _textures.end())
		return nullptr; // texture already exists

	auto texture = std::make_shared<Texture>(GL_TEXTURE_2D, format);
	if (_initalized)
		texture->Init(_resolution);

	_textures.emplace(name, texture);
	return texture;
}

std::shared_ptr<Texture> RenderingPipeline::CreateTexture(const xml::Element* properties)
{
	std::string name, format, target;
	if (!xml::GetAttribute(properties, "name", name))
		return nullptr;

	xml::GetAttribute(properties, "format", format);
	
	return CreateTexture(name, StringToTextureFormat(format));
}

RenderingPipeline::ViewPtr RenderingPipeline::CreateView(
	const std::string& textureName)
{
	TexturePtr texture = GetTexture(textureName);
	if (!texture)
		return nullptr;

	_views.push_back(std::make_shared<TextureView>(texture));
	if (_initalized)
		_views.back()->Init();

	return _views.back();
}

void RenderingPipeline::Update()
{
	glViewport(0, 0, _resolution.x, _resolution.y);
	for (auto stage : _stages)
		stage->DoRendering();
}

void RenderingPipeline::AddRenderingStage(RenderingStagePtr stage)
{
	_stages.push_back(stage);
}

void RenderingPipeline::AddRenderer(RendererPtr renderer)
{
	_renderer.push_back(renderer);
}

std::vector<std::shared_ptr<Renderer>>& RenderingPipeline::GetRenderer()
{
	return _renderer;
}

std::vector<std::shared_ptr<TextureView>>& RenderingPipeline::GetViews()
{
	return _views;
}

void RenderingPipeline::InitTextures(const xml::Element* properties)
{
	if (!properties)
		return;

	for (auto textureProperties : xml::IterateChildElements(properties, "Texture"))
	{
		CreateTexture(textureProperties);
	}
}

void RenderingPipeline::InitStages(const xml::Element* properties)
{
	if (!properties)
		return;

	for (auto stageProperties : xml::IterateChildElements(properties))
	{
		RenderingStagePtr stage =
			_grapicSys->GetRendertingSrageFactory().CreateObject(
				stageProperties->Value(), stageProperties, this);
		if (stage)
			_stages.push_back(stage);
	}
}

void RenderingPipeline::InitRenderer(const xml::Element* properties)
{
	if (!properties)
		return;

	for (auto rendererProperties : xml::IterateChildElements(properties))
	{
		RendererPtr renderer =
			_grapicSys->GetRendererFactory().CreateObject(
				rendererProperties->Value(), rendererProperties);

		if (renderer)
			_renderer.push_back(renderer);
	}
}

void RenderingPipeline::InitViews(const xml::Element* properties)
{
	if (!properties)
		return;

	for (auto viewProperties : xml::IterateChildElements(properties, "View"))
	{
		std::string texture;
		xml::GetAttribute(viewProperties, "texture", texture);

		auto view = CreateView(texture);
		float l = 0, r = 1, b = 0, t = 1;
		xml::GetAttribute(viewProperties, "left", l);
		xml::GetAttribute(viewProperties, "right", r);
		xml::GetAttribute(viewProperties, "bottom", b);
		xml::GetAttribute(viewProperties, "top", t);
		view->SetViewArea(glm::vec2(l, b), glm::vec2(r, t));
	}
}

void RenderingPipeline::InitCamera(const xml::Element* properties)
{
	if (!properties)
		return;

	glm::vec3 pos;
	xml::GetAttribute(properties, "Position", "x", pos.x);
	xml::GetAttribute(properties, "Position", "y", pos.y);
	xml::GetAttribute(properties, "Position", "z", pos.z);
	_camera->SetPosition(pos);

	float fov = 60, ar = (4/3), near = 0.1f, far = 1000.0f;
	xml::GetAttribute(properties, "Projection", "fild_of_view", fov);
	xml::GetAttribute(properties, "Projection", "aspect_ration", ar);
	xml::GetAttribute(properties, "Projection", "near", near);
	xml::GetAttribute(properties, "Projection", "far", far);
	fov *= 3.14159265359f/180.0f;
	_camera->SetUpProjection(fov, ar, near, far);
}

GLenum RenderingPipeline::StringToTextureFormat(const std::string& format)
{
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
		{"GL_RGB32F", GL_RGB32F}
	};
	auto it = mapping.find(format);
	if (it != mapping.end())
		return it->second;

	return GL_RGBA;
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
