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
	, fbo_(std::make_unique<FrameBufferObject>())
{}

DeferredLightingStage::~DeferredLightingStage()
{}

void DeferredLightingStage::SetDirLightShader(ShaderProgramPtr shader)
{
	dir_light_shader_ = shader;
	SetTextureSamplerUniforms(dir_light_shader_.get());
}
void DeferredLightingStage::SetPointLightShader(ShaderProgramPtr shader)
{
	point_light_shader_ = shader;
	SetTextureSamplerUniforms(point_light_shader_.get());
}

bool DeferredLightingStage::InitShader(
	const std::string& point_light_vert,
	const std::string& point_light_frag,
	const std::string& dir_light_vert,
	const std::string& dir_light_frag)
{
	point_light_shader_ = std::make_shared<Shader>();
	point_light_shader_->AttachShaderFromFile(
		GL_VERTEX_SHADER, point_light_vert);
	point_light_shader_->AttachShaderFromFile(
		GL_FRAGMENT_SHADER, point_light_frag);

	dir_light_shader_ = std::make_shared<Shader>();
	dir_light_shader_->AttachShaderFromFile(
		GL_VERTEX_SHADER, dir_light_vert);
	dir_light_shader_->AttachShaderFromFile(
		GL_FRAGMENT_SHADER, dir_light_frag);

	if( !point_light_shader_->Link() || !dir_light_shader_->Link() )
		return false;

	return true;
}

void DeferredLightingStage::SetOutputTexture(const std::string& texture)
{
	fbo_->SetColorAttechment(0, pipline_->GetTexture(texture));
}

void DeferredLightingStage::SetInputTextures(
	const std::vector<std::string>& textures)
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

	SetTextureSamplerUniforms(dir_light_shader_.get());
	SetTextureSamplerUniforms(point_light_shader_.get());
}

void DeferredLightingStage::SetScene(ScenePtr scene)
{
	scene_ = scene;
	dir_light_container_ =
		scene_->RequestComponentContainer<DirLightProperties>();
	point_light_container_ =
		scene_->RequestComponentContainer<PointLightProperties>();
	position_container_ =
		scene_->RequestComponentContainer<PositionComponent>();
}

void DeferredLightingStage::Init()
{
	fbo_->Init();
	buffer_size_ = 32;
	glGenBuffers(1, &vbo_);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_);
	glBufferData(GL_ARRAY_BUFFER,
		buffer_size_*8*sizeof(float), nullptr, GL_STREAM_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenVertexArrays(1, &vao_);
	glBindVertexArray(vao_);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_);
	glVertexAttribPointer(
		0, 3, GL_FLOAT, GL_FALSE,
		8*sizeof(float), (void*) (0*sizeof(float)));// position
	glVertexAttribPointer(
		1, 1, GL_FLOAT, GL_FALSE,
		8*sizeof(float), (void*) (3*sizeof(float)));// bulbRadius
	glVertexAttribPointer(
		2, 3, GL_FLOAT, GL_FALSE,
		8*sizeof(float), (void*) (4*sizeof(float)));// color
	glVertexAttribPointer(
		3, 1, GL_FLOAT, GL_FALSE,
		8*sizeof(float), (void*) (7*sizeof(float)));// cutoffRadius
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glVertexAttribDivisor(0, 1);
	glVertexAttribDivisor(1, 1);
	glVertexAttribDivisor(2, 1);
	glVertexAttribDivisor(3, 1);

	glBindVertexArray(0);
}

void DeferredLightingStage::Cleanup()
{
	fbo_->CleanUp();
}

void DeferredLightingStage::DoRendering()
{
	if( !scene_ )
		return;

	fbo_->BeginRenderToTexture();
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

	fbo_->EndRenderToTexture();
}

std::unique_ptr<DeferredLightingStage> DeferredLightingStage::CreateInstance(
	const xml::Element* properties, RenderingPipeline* pipline)
{
	auto dir_light_shader = CreateShader(properties->FirstChildElement("DirectionalLightProgram"));
	auto point_light_shader = CreateShader(properties->FirstChildElement("PointLightShaderProgram"));

	auto stage = std::make_unique<DeferredLightingStage>(pipline);
	stage->SetDirLightShader(dir_light_shader);
	stage->SetPointLightShader(point_light_shader);

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

std::shared_ptr<Shader> DeferredLightingStage::CreateShader(const xml::Element* properties)
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

void DeferredLightingStage::SetTextureSamplerUniforms(Shader* shader)
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
					glUniform1i(localtion, n);
			}
		}
	}
	glUseProgram(0);
}

void DeferredLightingStage::BindTextures()
{
	for( size_t n = 0; n<input_textures_.size(); ++n )
	{
		if( input_textures_[n] )
		{
			glActiveTexture(GL_TEXTURE0 + n);
			input_textures_[n]->Bind();
		}
	}
}

void DeferredLightingStage::ReleaseTextures()
{
	for( size_t n = input_textures_.size(); n>0; --n )
	{
		if( input_textures_[n-1] )
		{
			glActiveTexture(GL_TEXTURE0 + n-1);
			input_textures_[n-1]->Release();
		}
	}
}

void DeferredLightingStage::ApplyDirectionalLights()
{
	Scene::ComponentMask mask =
		dir_light_container_->GetComponentMask();

	dir_light_shader_->Use();

	glm::mat4 inv_projection = pipline_->GetCamera()->GetinverseProjectionMatrix();

	glUniformMatrix4fv(
		dir_light_shader_->GetUniformLocation("uInverseProjectionMatrix"),
		1, GL_FALSE, &inv_projection[0][0]);

	glm::mat3 normal_matrix = pipline_->GetCamera()->GetViewMatrix();

	for( const auto& it : dir_light_container_->GetComponents() )
	{
		glm::vec3 dir = it.second.direction;
		dir = normal_matrix*dir;

		glUniform3f(
			dir_light_shader_->GetUniformLocation("uLigthColor"),
			it.second.color.r,
			it.second.color.g,
			it.second.color.b);
		glUniform3f(
			dir_light_shader_->GetUniformLocation("uLigthDir"),
			dir.x, dir.y, dir.z);

		if( (scene_->GetEntitys()[it.first] & mask)!=mask )
			continue;// this should not happen

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}

	glUseProgram(0);
}

struct PointLight
{
	float x, y, z;
	float bulb_radius;
	float r, g, b;
	float cutoff_radius;
};

void DeferredLightingStage::ApplyPointLights()
{
	Scene::ComponentMask mask =
		point_light_container_->GetComponentMask() |
		position_container_->GetComponentMask();

	Camera* camera = pipline_->GetCamera().get();

	std::vector<PointLight> point_lights;
	for( const auto& it : point_light_container_->GetComponents() )
	{
		if( (scene_->GetEntitys()[it.first] & mask)!=mask )
			continue;// this should not happen

		PointLight light;

		glm::vec4 position = glm::vec4(
			position_container_->GetComponent(it.first)->position, 1);
		position = camera->GetViewMatrix()*position;

		light.x = position.x;
		light.y = position.y;
		light.z = position.z;

		light.bulb_radius = it.second.bulb_radius;

		light.r = it.second.color.r;
		light.g = it.second.color.g;
		light.b = it.second.color.b;

		light.cutoff_radius = it.second.cutoff_radius;
		point_lights.push_back(light);
	}

	point_light_shader_->Use();
	glBindVertexArray(vao_);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_);

	glm::mat4 projection = camera->GetProjectionMatrix();
	glm::mat4 invers_projection = camera->GetinverseProjectionMatrix();

	glUniformMatrix4fv(
		point_light_shader_->GetUniformLocation("uProjectionMatrix"),
		1, GL_FALSE, &projection[0][0]);
	glUniformMatrix4fv(
		point_light_shader_->GetUniformLocation("uInverseProjectionMatrix"),
		1, GL_FALSE, &invers_projection[0][0]);
	glUniform2f(
		point_light_shader_->GetUniformLocation("uTexelSize"),
		1.0f/pipline_->GetResulution().x,
		1.0f/pipline_->GetResulution().y);

	for( size_t n = 0; n<point_lights.size(); n += buffer_size_ )
	{
		GLsizei num_lights = std::min(
			buffer_size_, unsigned(point_lights.size()-n));
		glBufferSubData(
			GL_ARRAY_BUFFER, 0, num_lights*sizeof(PointLight), &point_lights[n]);

		glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, 10, num_lights);
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glUseProgram(0);
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
