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
	, fbo_(std::make_unique<FrameBufferObject>())
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
		static_cast<GLint>(GL_SRGB8),
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
	environment_map_ = std::make_shared<Texture>(
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

void EnvironmentMapReflectionStage::SetShader(ShaderPtr shader)
{
	shader_ = shader;
}

void EnvironmentMapReflectionStage::SetOutputTexture(const std::string& texture)
{
	fbo_->SetColorAttechment(0, pipline_->GetTexture(texture));
}

void EnvironmentMapReflectionStage::SetInputTextures(const std::vector<std::string>& textures)
{
	input_textures_.clear();
	input_textur_names_.clear();
	for( const std::string& texture_name : textures )
	{
		auto texture = pipline_->GetTexture(texture_name);
		if( !texture )
			continue;

		input_textures_.push_back(texture);
		input_textur_names_.push_back(texture_name);
	}

	SetTextureSamplerUniforms(shader_.get());
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
	if( !shader_ )
		return;

	fbo_->BeginRenderToTexture();

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);

	shader_->Use();

	glm::mat4 proj = pipline_->GetCamera()->GetProjectionMatrix();
	glm::mat4 view = pipline_->GetCamera()->GetViewMatrix();
	glm::mat4 invProj = glm::inverse(proj);
	glm::mat4 invView = glm::inverse(view);

	glUniformMatrix4fv(
		shader_->GetUniformLocation("uInverseProjectionMatrix"),
		1, GL_FALSE, &invProj[0][0]);
	glUniformMatrix4fv(
		shader_->GetUniformLocation("uInverseViewMatrix"),
		1, GL_FALSE, &invView[0][0]);

	BindTextures();

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	ReleaseTextures();

	glDisable(GL_BLEND);
	fbo_->EndRenderToTexture();
}

std::unique_ptr<EnvironmentMapReflectionStage> EnvironmentMapReflectionStage::CreateInstance(
	const xml::Element* properties, RenderingPipeline* pipline)
{
	auto shader = CreateShader(properties->FirstChildElement("Shader"));

	auto stage = std::make_unique<EnvironmentMapReflectionStage>(pipline);
	stage->SetShader(shader);

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

void EnvironmentMapReflectionStage::SetTextureSamplerUniforms(Shader* shader)
{
	if( shader == nullptr )
		return;
	if( input_textur_names_.empty() )
		return;

	const static std::vector<std::string> allowed_prefixes =
	{"u", "uTex", "uTexture", "uSampler"};
	const static std::vector<std::string> allowed_postfixes =
	{"", "Buffer", "Texture" "Sampler"};

	shader->Use();
	// search for uniform with name 'u{textureName}'
	for( size_t n = 0; n<input_textur_names_.size(); ++n )
	{
		std::string uniformName = input_textur_names_[n];
		uniformName[0] = std::toupper(uniformName[0]);

		for( const std::string& prefix : allowed_prefixes )
		{
			for( const std::string& postfix : allowed_postfixes )
			{
				int localtion = shader->GetUniformLocation(prefix + uniformName + postfix);
				if( localtion >= 0 )
					glUniform1i(localtion, n+1);
			}
		}
	}
	glUseProgram(0);
}

void EnvironmentMapReflectionStage::BindTextures()
{
	glActiveTexture(GL_TEXTURE0);
	environment_map_->Bind();

	for( size_t n = 0; n<input_textures_.size(); ++n )
	{
		if( input_textures_[n] )
		{
			glActiveTexture(GL_TEXTURE1 + n);
			input_textures_[n]->Bind();
		}
	}
}

void EnvironmentMapReflectionStage::ReleaseTextures()
{
	glActiveTexture(GL_TEXTURE0);
	environment_map_->Release();

	for( size_t n = 0; n<input_textures_.size(); ++n )
	{
		if( input_textures_[n] )
		{
			glActiveTexture(GL_TEXTURE1 + n);
			input_textures_[n]->Release();
		}
	}
	glActiveTexture(GL_TEXTURE0);
}

std::shared_ptr<Shader> EnvironmentMapReflectionStage::CreateShader(
	const xml::Element* properties)
{
	if( !properties )
		return nullptr;

	std::string file_name;

	auto program = std::make_shared<Shader>();
	for( auto shader : xml::IterateChildElements(properties, "VertexShader") )
	{
		if( xml::GetAttribute(shader, "file", file_name) )
			program->AttachShaderFromFile(GL_VERTEX_SHADER, file_name);
	}
	for( auto shader : xml::IterateChildElements(properties, "FragmentShader") )
	{
		if( xml::GetAttribute(shader, "file", file_name) )
			program->AttachShaderFromFile(GL_FRAGMENT_SHADER, file_name);
	}

	if( !program->Link() )
		return nullptr;

	return program;
}
} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
