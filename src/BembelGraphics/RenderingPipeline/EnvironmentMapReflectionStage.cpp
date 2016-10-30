/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "EnvironmentMapReflectionStage.h"
#include "RenderingPipeline.h"
#include "Camera.h"

#include <BembelKernel/Rendering/Texture.h>
#include <BembelKernel/Rendering/Shader.h>
#include <BembelKernel/Rendering/FrameBufferObject.h>

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel {

EnvironmentMapReflectionStage::EnvironmentMapReflectionStage(RenderingPipeline* pipline)
	: RenderingStage(pipline)
	, _fbo(std::make_unique<FrameBufferObject>())
{}

EnvironmentMapReflectionStage::~EnvironmentMapReflectionStage()
{}

namespace{
bool LoadCubeMapSide(GLenum target, const std::string& file)
{
	Image image;
	if (!image.Load(file))
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
	_environmentMap = std::make_shared<Texture>(
		GL_TEXTURE_CUBE_MAP, GL_RGBA);
	_environmentMap->Bind();

	if (!LoadCubeMapSide(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, left)   ||
		!LoadCubeMapSide(GL_TEXTURE_CUBE_MAP_POSITIVE_X, right)  ||
		!LoadCubeMapSide(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, bottom) ||
		!LoadCubeMapSide(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, top)    ||
		!LoadCubeMapSide(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, front)   ||
		!LoadCubeMapSide(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, back) )
	{
		_environmentMap.reset();
		return false;
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GLint(GL_LINEAR));
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GLint(GL_LINEAR_MIPMAP_LINEAR));
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GLint(GL_CLAMP_TO_EDGE));
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GLint(GL_CLAMP_TO_EDGE));
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GLint(GL_CLAMP_TO_EDGE));
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	_environmentMap->Release();

	return true;
}

void EnvironmentMapReflectionStage::SetShader(ShaderPtr shader)
{
	_shader = shader;
}

void EnvironmentMapReflectionStage::SetOutputTexture(const std::string& texture)
{
	_fbo->SetColorAttechment(0, _pipline->GetTexture(texture));
}

void EnvironmentMapReflectionStage::SetInputTextures(const std::vector<std::string>& textures)
{
	_inputTextures.clear();
	_inputTexturNames.clear();
	for (const std::string& textureName : textures)
	{
		auto texture = _pipline->GetTexture(textureName);
		if (!texture)
			continue;

		_inputTextures.push_back(texture);
		_inputTexturNames.push_back(textureName);
	}

	SetTextureSamplerUniforms(_shader.get());
}

void EnvironmentMapReflectionStage::Init()
{
	_fbo->Init();
}

void EnvironmentMapReflectionStage::Cleanup()
{
	_fbo->CleanUp();
}

void EnvironmentMapReflectionStage::DoRendering()
{
	if (!_shader)
		return;

	_fbo->BeginRenderToTexture();

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);

	_shader->Use(); 

	glm::mat4 proj = _pipline->GetCamera()->GetProjectionMatrix();
	glm::mat4 view = _pipline->GetCamera()->GetViewMatrix();
	glm::mat4 invProj = glm::inverse(proj);
	glm::mat4 invView = glm::inverse(view);

	glUniformMatrix4fv(
		_shader->GetUniformLocation("uInverseProjectionMatrix"),
		1, GL_FALSE, &invProj[0][0]);
	glUniformMatrix4fv(
		_shader->GetUniformLocation("uInverseViewMatrix"),
		1, GL_FALSE, &invView[0][0]);

	BindTextures();

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	ReleaseTextures();

	glDisable(GL_BLEND);
	_fbo->EndRenderToTexture();
}

std::unique_ptr<EnvironmentMapReflectionStage> EnvironmentMapReflectionStage::CreateInstance(
	const xml::Element* properties, RenderingPipeline* pipline)
{
	auto shader = CreateShader(properties->FirstChildElement("Shader"));

	auto stage = std::make_unique<EnvironmentMapReflectionStage>(pipline);
	stage->SetShader(shader);

	const xml::Element* envMap = properties->FirstChildElement("EnvironmentMap");
	std::string left, right, bottom, top, back, front;
	xml::GetAttribute(envMap, "left", left);
	xml::GetAttribute(envMap, "right", right);
	xml::GetAttribute(envMap, "bottom", bottom);
	xml::GetAttribute(envMap, "top", top);
	xml::GetAttribute(envMap, "back", back);
	xml::GetAttribute(envMap, "front", front);
	if (!stage->InitEnvironmentMap(left, right, bottom, top, back, front))
		return nullptr;

	std::string textureName;
	if (xml::GetAttribute(properties, "Output", "texture", textureName))
		stage->SetOutputTexture(textureName);

	std::vector<std::string> intputTextures;
	for (auto input : xml::IterateChildElements(properties, "Input"))
	{
		if (xml::GetAttribute(input, "texture", textureName))
			intputTextures.push_back(textureName);
	}
	stage->SetInputTextures(intputTextures);


	return std::move(stage);
}

void EnvironmentMapReflectionStage::SetTextureSamplerUniforms(Shader* shader)
{
	if (shader == nullptr)
		return;
	if (_inputTexturNames.empty())
		return;

	const static std::vector<std::string> allowedPrefixes =
	{"u", "uTex", "uTexture", "uSampler"};
	const static std::vector<std::string> allowedPostfixes =
	{"", "Buffer", "Texture" "Sampler"};

	shader->Use();
	// search for uniform with name 'u{textureName}'
	for (size_t n = 0; n<_inputTexturNames.size(); ++n)
	{
		std::string uniformName = _inputTexturNames[n];
		uniformName[0] = std::toupper(uniformName[0]);

		for (const std::string& prefix : allowedPrefixes)
		{
			for (const std::string& postfix : allowedPostfixes)
			{
				int localtion = shader->GetUniformLocation(prefix + uniformName + postfix);
				if (localtion >= 0)
					glUniform1i(localtion, n+1);
			}
		}
	}
	glUseProgram(0);
}

void EnvironmentMapReflectionStage::BindTextures()
{
	glActiveTexture(GL_TEXTURE0);
	_environmentMap->Bind();

	for (size_t n = 0; n<_inputTextures.size(); ++n)
	{
		if (_inputTextures[n])
		{
			glActiveTexture(GL_TEXTURE1 + n);
			_inputTextures[n]->Bind();
		}
	}
}

void EnvironmentMapReflectionStage::ReleaseTextures()
{
	glActiveTexture(GL_TEXTURE0);
	_environmentMap->Release();

	for (size_t n = 0; n<_inputTextures.size(); ++n)
	{
		if (_inputTextures[n])
		{
			glActiveTexture(GL_TEXTURE1 + n);
			_inputTextures[n]->Release();
		}
	}
	glActiveTexture(GL_TEXTURE0);
}

std::shared_ptr<Shader> EnvironmentMapReflectionStage::CreateShader(const xml::Element* properties)
{
	if (!properties)
		return nullptr;

	std::string filename;

	auto program = std::make_shared<Shader>();
	for (auto shader : xml::IterateChildElements(properties, "VertexShader"))
	{
		if (xml::GetAttribute(shader, "file", filename))
			program->AttachShaderFromFile(GL_VERTEX_SHADER, filename);
	}
	for (auto shader : xml::IterateChildElements(properties, "FragmentShader"))
	{
		if (xml::GetAttribute(shader, "file", filename))
			program->AttachShaderFromFile(GL_FRAGMENT_SHADER, filename);
	}

	if (!program->Link())
		return nullptr;

	return program;
}
} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
