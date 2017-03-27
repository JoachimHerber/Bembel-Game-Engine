/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "environment-map-reflection-stage.h"
#include "rendering-pipeline.h"
#include "camera.h"

#include <bembel-kernel/rendering/texture.h>
#include <bembel-kernel/rendering/shader.h>
#include <bembel-kernel/rendering/frame-buffer-object.h>

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel {

EnvironmentMapReflectionStage::EnvironmentMapReflectionStage(RenderingPipeline* pipline)
	: RenderingStage(pipline)
{}

EnvironmentMapReflectionStage::~EnvironmentMapReflectionStage()
{}

namespace {
bool LoadCubeMapSide(GLenum target, const std::string& file)
{
	Image image;
	if( !image.Load(file, false) )
		return false;

	glTexImage2D(
		target, 0,
		static_cast<GLint>(GL_RGB8),
		image.GetWidth(), image.GetHeight(), 0,
		image.GetChannels() == 4 ? GL_RGBA : GL_RGB,
		GL_UNSIGNED_BYTE, image.GetData());
	return true;
}
}


bool EnvironmentMapReflectionStage::InitEnvironmentMap(
	const std::string& left,
	const std::string& right,
	const std::string& bottom,
	const std::string& top,
	const std::string& back,
	const std::string& front)
{
	environment_map_ = std::make_unique<Texture>(
		GL_TEXTURE_CUBE_MAP, GL_RGBA);
	environment_map_->Bind();

	if( !LoadCubeMapSide(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, left)   ||
		!LoadCubeMapSide(GL_TEXTURE_CUBE_MAP_POSITIVE_X, right)  ||
		!LoadCubeMapSide(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, bottom) ||
		!LoadCubeMapSide(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, top)    ||
		!LoadCubeMapSide(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, front)   ||
		!LoadCubeMapSide(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, back) )
	{
		environment_map_.reset();
		return false;
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GLint(GL_LINEAR));
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GLint(GL_LINEAR_MIPMAP_LINEAR));
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GLint(GL_CLAMP_TO_EDGE));
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GLint(GL_CLAMP_TO_EDGE));
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GLint(GL_CLAMP_TO_EDGE));
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	environment_map_->Release();

	return true;
}

void EnvironmentMapReflectionStage::SetShader(AssetHandle shader)
{
	shader_program_ = shader;
}

void EnvironmentMapReflectionStage::SetOutputTexture(const std::string& texture)
{
	fbo_->SetColorAttechment(0, pipline_->GetTexture(texture));
}

void EnvironmentMapReflectionStage::Init()
{
	fbo_->Init();
}

void EnvironmentMapReflectionStage::Cleanup()
{
	fbo_->CleanUp();
}

void EnvironmentMapReflectionStage::DoRendering()
{
	auto program = GetAssetManager()->GetAsset<ShaderProgram>(shader_program_);
	if( !program )
		return;

	fbo_->BeginRenderToTexture();

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);

	program->Use();

	glm::mat4 proj = pipline_->GetCamera()->GetProjectionMatrix();
	glm::mat4 view = pipline_->GetCamera()->GetViewMatrix();
	glm::mat4 invProj = glm::inverse(proj);
	glm::mat4 invView = glm::inverse(view);

	glUniformMatrix4fv(
		program->GetUniformLocation("uInverseProjectionMatrix"),
		1, GL_FALSE, &invProj[0][0]);
	glUniformMatrix4fv(
		program->GetUniformLocation("uInverseViewMatrix"),
		1, GL_FALSE, &invView[0][0]);

	BindInputTextures();

	glActiveTexture(GL_TEXTURE0 + textures_.size());
	environment_map_->Bind();

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	environment_map_->Release();

	ReleaseInputTextures();

	glDisable(GL_BLEND);
	fbo_->EndRenderToTexture();
}

std::unique_ptr<EnvironmentMapReflectionStage> EnvironmentMapReflectionStage::CreateInstance(
	const xml::Element* properties, RenderingPipeline* pipline)
{
	auto program = 
		pipline->GetAssetManager()->RequestAsset<ShaderProgram>(
			properties->FirstChildElement("Shader"));

	auto stage = std::make_unique<EnvironmentMapReflectionStage>(pipline);
	stage->SetShader(program);

	const xml::Element* env_map = properties->FirstChildElement("EnvironmentMap");
	std::string left, right, bottom, top, back, front;
	xml::GetAttribute(env_map, "left", left);
	xml::GetAttribute(env_map, "right", right);
	xml::GetAttribute(env_map, "bottom", bottom);
	xml::GetAttribute(env_map, "top", top);
	xml::GetAttribute(env_map, "back", back);
	xml::GetAttribute(env_map, "front", front);
	if( !stage->InitEnvironmentMap(left, right, bottom, top, back, front) )
		return nullptr;

	std::string texture_name;
	if( xml::GetAttribute(properties, "Output", "texture", texture_name) )
		stage->SetOutputTexture(texture_name);

	std::vector<std::string> intput_textures;
	for( auto input : xml::IterateChildElements(properties, "Input") )
	{
		if( xml::GetAttribute(input, "texture", texture_name) )
			intput_textures.push_back(texture_name);
	}
	stage->SetInputTextures(intput_textures);


	return std::move(stage);
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
