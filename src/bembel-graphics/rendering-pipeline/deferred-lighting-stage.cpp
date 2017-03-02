/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "deferred-lighting-stage.h"
#include "rendering-pipeline.h"
#include "camera.h"
#include "light-source-component.h"

#include <bembel-base/logging/logger.h>
#include <bembel-kernel/scene/position-component.h>
#include <bembel-kernel/rendering/texture.h>
#include <bembel-kernel/rendering/shader.h>
#include <bembel-kernel/rendering/frame-buffer-object.h>


/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel {

DeferredLightingStage::DeferredLightingStage(RenderingPipeline* pipline)
	: RenderingStage(pipline)
	, _fbo(std::make_unique<FrameBufferObject>())
{}

DeferredLightingStage::~DeferredLightingStage()
{}

void DeferredLightingStage::SetDirLightShader(ShaderProgramPtr shader)
{
	_dirLightShader = shader;
	SetTextureSamplerUniforms(_dirLightShader.get());
}
void DeferredLightingStage::SetPointLightShader(ShaderProgramPtr shader)
{
	_pointLightShader = shader;
	SetTextureSamplerUniforms(_pointLightShader.get());
}

bool DeferredLightingStage::InitShader(
	const std::string& pointLightVert, 
	const std::string& pointLightFrag, 
	const std::string& dirLightVert, 
	const std::string& dirLightFrag)
{
	_pointLightShader = std::make_shared<Shader>();
	_pointLightShader->AttachShaderFromFile(GL_VERTEX_SHADER, pointLightVert);
	_pointLightShader->AttachShaderFromFile(GL_FRAGMENT_SHADER, pointLightFrag);

	_dirLightShader = std::make_shared<Shader>();
	_dirLightShader->AttachShaderFromFile(GL_VERTEX_SHADER, dirLightVert);
	_dirLightShader->AttachShaderFromFile(GL_FRAGMENT_SHADER, dirLightFrag);

	if (!_pointLightShader->Link() || !_dirLightShader->Link())
		return false;

	return true;
}

void DeferredLightingStage::SetOutputTexture(const std::string& texture)
{
	_fbo->SetColorAttechment(0, _pipline->GetTexture(texture));
}

void DeferredLightingStage::SetInputTextures(const std::vector<std::string>& textures)
{
	_inputTextures.clear();
	_inputTexturNames.clear();
	for (const std::string& textureName : textures)
	{
		auto texture = _pipline->GetTexture(textureName);
		if(!texture)
			continue;

		_inputTextures.push_back(texture);
		_inputTexturNames.push_back(textureName);
	}

	SetTextureSamplerUniforms(_dirLightShader.get());
	SetTextureSamplerUniforms(_pointLightShader.get());
}

void DeferredLightingStage::SetScene(ScenePtr scene)
{
	_scene = scene;
	_dirLightContainer   = _scene->RequestComponentContainer<DirLightProperties>();
	_pointLightContainer = _scene->RequestComponentContainer<PointLightProperties>();
	_positionContainer   = _scene->RequestComponentContainer<PositionComponent>();
}

void DeferredLightingStage::Init()
{
	_fbo->Init();
	_bufferSize = 32;
	glGenBuffers(1, &_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, _bufferSize*8*sizeof(float), nullptr, GL_STREAM_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);

	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(0*sizeof(float)));// position
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3*sizeof(float)));// bulbRadius
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(4*sizeof(float)));// color
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(7*sizeof(float)));// cutoffRadius
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glVertexAttribDivisor(0, 1);
	glVertexAttribDivisor(1, 1);
	glVertexAttribDivisor(2, 1);
	glVertexAttribDivisor(3, 1);

	glBindVertexArray(0);
}

void DeferredLightingStage::Cleanup()
{
	_fbo->CleanUp();
}

void DeferredLightingStage::DoRendering()
{
	if (!_scene)
		return;

	_fbo->BeginRenderToTexture();
	//glClearColor(0, 0, 0, 0);
	//glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	glBlendFunc(GL_ONE, GL_ONE);
	glEnable(GL_BLEND);

	BindTextures();

	ApplyDirectionalLights();
	ApplyPointLights();

	ReleaseTextures();

	glDisable(GL_BLEND);

	_fbo->EndRenderToTexture();
}

std::unique_ptr<DeferredLightingStage> DeferredLightingStage::CreateInstance(
	const xml::Element* properties, RenderingPipeline* pipline)
{
	auto dirLightShader   = CreateShader(properties->FirstChildElement("DirectionalLightProgram"));
	auto pointLightShader = CreateShader(properties->FirstChildElement("PointLightShaderProgram"));

	auto stage = std::make_unique<DeferredLightingStage>(pipline);
	stage->SetDirLightShader(dirLightShader);
	stage->SetPointLightShader(pointLightShader);

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

std::shared_ptr<Shader> DeferredLightingStage::CreateShader(const xml::Element* properties)
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

void DeferredLightingStage::SetTextureSamplerUniforms(Shader* shader)
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
					glUniform1i(localtion, n);
			}
		}
	}
	glUseProgram(0);
}

void DeferredLightingStage::BindTextures()
{
	for (size_t n = 0; n<_inputTextures.size(); ++n)
	{
		if (_inputTextures[n])
		{
			glActiveTexture(GL_TEXTURE0 + n);
			_inputTextures[n]->Bind();
		}
	}
}

void DeferredLightingStage::ReleaseTextures()
{
	for (size_t n = _inputTextures.size(); n>0; --n)
	{
		if (_inputTextures[n-1])
		{
			glActiveTexture(GL_TEXTURE0 + n-1);
			_inputTextures[n-1]->Release();
		}
	}
}

void DeferredLightingStage::ApplyDirectionalLights()
{
	Scene::ComponentMask mask =
		_dirLightContainer->GetComponentMask();

	_dirLightShader->Use();

	glm::mat4 projection = _pipline->GetCamera()->GetProjectionMatrix();
	glm::mat4 invProjection = glm::inverse(projection);

	glUniformMatrix4fv(
		_dirLightShader->GetUniformLocation("uInverseProjectionMatrix"),
		1, GL_FALSE, &invProjection[0][0]);

	glm::mat3 normalMatrix = _pipline->GetCamera()->GetViewMatrix();

	for (const auto& it : _dirLightContainer->GetComponents())
	{
		glm::vec3 dir = it.second.direction;
		dir = normalMatrix*dir;

		glUniform3f(
			_dirLightShader->GetUniformLocation("uLigthColor"),
			it.second.color.r,
			it.second.color.g,
			it.second.color.b);
		glUniform3f(
			_dirLightShader->GetUniformLocation("uLigthDir"),
			dir.x, dir.y, dir.z);

		if ((_scene->GetEntitys()[it.first] & mask)!=mask)
			continue;// this should not happen

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}



	glUseProgram(0);
}

struct PointLight
{
	float x, y, z;
	float bulbRadius;
	float r, g, b;
	float cutoffRadius;
};

void DeferredLightingStage::ApplyPointLights()
{
	Scene::ComponentMask mask =
		_pointLightContainer->GetComponentMask() |
		_positionContainer->GetComponentMask();

	std::vector<PointLight> pointLights;
	for (const auto& it : _pointLightContainer->GetComponents())
	{
		if ((_scene->GetEntitys()[it.first] & mask)!=mask)
			continue;// this should not happen

		PointLight light;

		glm::vec4 position = glm::vec4(_positionContainer->GetComponent(it.first)->position, 1);
		position = _pipline->GetCamera()->GetViewMatrix()*position;

		light.x = position.x;
		light.y = position.y;
		light.z = position.z;

		light.bulbRadius = it.second.bulbRadius;

		light.r = it.second.color.r;
		light.g = it.second.color.g;
		light.b = it.second.color.b;

		light.cutoffRadius = it.second.cutoffRadius;
		pointLights.push_back(light);
	}

	_pointLightShader->Use();
	glBindVertexArray(_vao);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);

	glm::mat4 projection    = _pipline->GetCamera()->GetProjectionMatrix();
	glm::mat4 invProjection = glm::inverse(projection);

	glUniformMatrix4fv(
		_pointLightShader->GetUniformLocation("uProjectionMatrix"),
		1, GL_FALSE, &projection[0][0]);
	glUniformMatrix4fv(
		_pointLightShader->GetUniformLocation("uInverseProjectionMatrix"),
		1, GL_FALSE, &invProjection[0][0]);
	glUniform2f(
		_pointLightShader->GetUniformLocation("uTexelSize"),
		1.0f/_pipline->GetResulution().x, 
		1.0f/_pipline->GetResulution().y);

	for (size_t n = 0; n<pointLights.size(); n += _bufferSize)
	{
		GLsizei numLights = std::min(_bufferSize, unsigned(pointLights.size()-n));
		glBufferSubData(GL_ARRAY_BUFFER, 0, numLights*sizeof(PointLight), &pointLights[n]);

		glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, 10, numLights);
	}
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glUseProgram(0);
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
