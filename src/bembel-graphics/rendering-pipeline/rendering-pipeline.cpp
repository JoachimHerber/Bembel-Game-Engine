/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "rendering-pipeline.h"
#include "rendering-stage.h"
#include "camera.h"

#include "../texture-view.h"
#include "../graphic-system.h"
#include "../geometry/geometry-renderer.h"

#include <bembel-kernel/rendering/texture.h>
#include <bembel-kernel/rendering/shader.h>
#include <bembel-kernel/rendering/frame-buffer-object.h>

/*============================================================================*/
/* IMPLEMENTATION			                                                  */
/*============================================================================*/
namespace bembel {

RenderingPipeline::RenderingPipeline(GraphicSystem* grapic_system)
	: grapic_system_(grapic_system)
	, resolution_(800, 600)
	, enabled_(true)
	, initalized_(false)
	, scene_(nullptr)
	, camera_(std::make_shared<Camera>())
{}

RenderingPipeline::~RenderingPipeline()
{}

GraphicSystem* RenderingPipeline::GetGraphicSystem() const
{
	return grapic_system_;
}

void RenderingPipeline::Enable()
{
	enabled_ = true;
}
void RenderingPipeline::Disable()
{
	enabled_ = false;
}
bool RenderingPipeline::IsEnabled()
{
	return enabled_;
}

void RenderingPipeline::SetResulution(const glm::ivec2& value)
{
	if( resolution_ == value )
		return;

	resolution_ = value;
	if( initalized_ )
	{
		for( auto& it : textures_ )
			it.second->SetSize(value);
	}
}

const glm::ivec2& RenderingPipeline::GetResulution() const
{
	return resolution_;
}

void RenderingPipeline::Init()
{
	for( auto& it : textures_ )
		it.second->Init(resolution_);

	for( auto& stage : stages_ )
		stage->Init();

	for( auto& view : views_ )
		view->Init();
}

bool RenderingPipeline::Configure(const xml::Element* properties)
{
	if( !properties )
		return false;

	xml::GetAttribute(properties, "horizontal_resolution", resolution_.x);
	xml::GetAttribute(properties, "vertical_resolution", resolution_.y);

	InitTextures(properties->FirstChildElement("Textures"));
	InitStages(properties->FirstChildElement("RenderingStages"));
	InitViews(properties->FirstChildElement("Views"));
	InitCamera(properties->FirstChildElement("Camera"));
	return true;
}

void RenderingPipeline::Cleanup()
{
	for( auto& view : views_ )
		view->Cleanup();

	for( auto& stage : stages_ )
		stage->Cleanup();

	for( auto& it : textures_ )
		it.second->Cleanup();
}

void RenderingPipeline::SetScene(ScenePtr scene)
{
	scene_ = scene;

	for( auto& stage : stages_ )
		stage->SetScene(scene_);
}

std::shared_ptr<Scene> RenderingPipeline::GetScene() const
{
	return scene_;
}

std::shared_ptr<Camera> RenderingPipeline::GetCamera() const
{
	return camera_;
}

std::shared_ptr<Texture> RenderingPipeline::GetTexture(
	const std::string& name) const
{
	auto it = textures_.find(name);
	if( it != textures_.end() )
		return it->second;

	return nullptr;
}
std::shared_ptr<Texture> RenderingPipeline::CreateTexture(
	const std::string& name,
	GLenum format)
{
	if( textures_.find(name) != textures_.end() )
		return nullptr; // texture already exists

	auto texture = std::make_shared<Texture>(GL_TEXTURE_2D, format);
	if( initalized_ )
		texture->Init(resolution_);

	textures_.emplace(name, texture);
	return texture;
}

std::shared_ptr<Texture> RenderingPipeline::CreateTexture(
	const xml::Element* properties)
{
	std::string name, format, target;
	if( !xml::GetAttribute(properties, "name", name) )
		return nullptr;

	xml::GetAttribute(properties, "format", format);

	return CreateTexture(name, StringToTextureFormat(format));
}

RenderingPipeline::ViewPtr RenderingPipeline::CreateView(
	const std::string& texture_name)
{
	TexturePtr texture = GetTexture(texture_name);
	if( !texture )
		return nullptr;

	views_.push_back(std::make_shared<TextureView>(texture));
	if( initalized_ )
		views_.back()->Init();

	return views_.back();
}

void RenderingPipeline::Update()
{
	glViewport(0, 0, resolution_.x, resolution_.y);
	for( auto stage : stages_ )
		stage->DoRendering();
}

void RenderingPipeline::AddRenderingStage(RenderingStagePtr stage)
{
	stages_.push_back(stage);
}

std::vector<std::shared_ptr<TextureView>>& RenderingPipeline::GetViews()
{
	return views_;
}

void RenderingPipeline::InitTextures(const xml::Element* properties)
{
	if( !properties )
		return;

	for( auto textureProperties : xml::IterateChildElements(properties, "Texture") )
	{
		CreateTexture(textureProperties);
	}
}

void RenderingPipeline::InitStages(const xml::Element* properties)
{
	if( !properties )
		return;

	for( auto stageProperties : xml::IterateChildElements(properties) )
	{
		RenderingStagePtr stage =
			grapic_system_->GetRendertingStageFactory().CreateObject(
				stageProperties->Value(), stageProperties, this);
		if( stage )
			stages_.push_back(stage);
	}
}

void RenderingPipeline::InitViews(const xml::Element* properties)
{
	if( !properties )
		return;

	for( auto viewProperties : xml::IterateChildElements(properties, "View") )
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
	if( !properties )
		return;

	glm::vec3 pos;
	xml::GetAttribute(properties, "Position", "x", pos.x);
	xml::GetAttribute(properties, "Position", "y", pos.y);
	xml::GetAttribute(properties, "Position", "z", pos.z);
	camera_->SetPosition(pos);

	float fov = 60, ar = (4/3), near = 0.1f, far = 1000.0f;
	xml::GetAttribute(properties, "Projection", "fild_of_view", fov);
	xml::GetAttribute(properties, "Projection", "aspect_ration", ar);
	xml::GetAttribute(properties, "Projection", "near", near);
	xml::GetAttribute(properties, "Projection", "far", far);
	fov *= 3.14159265359f/180.0f;
	camera_->SetUpProjection(fov, ar, near, far);
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
	if( it != mapping.end() )
		return it->second;

	return GL_RGBA;
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
