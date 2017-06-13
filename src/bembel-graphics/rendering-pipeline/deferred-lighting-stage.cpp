/******************************************************************************/
/* ************************************************************************** */
/* *                                                                        * */
/* *    MIT License                                                         * */
/* *                                                                        * */
/* *   Copyright(c) 2017 Joachim Herber                                     * */
/* *                                                                        * */
/* *   Permission is hereby granted, free of charge, to any person          * */
/* *   obtaining copy of this software and associated documentation files   * */
/* *   (the "Software"), to deal in the Software without restriction,       * */
/* *   including without limitation the rights to use, copy, modify, merge, * */
/* *   publish, distribute, sublicense, and/or sell copies of the Software, * */
/* *   and to permit persons to whom the Software is furnished to do so,    * */
/* *   subject to the following conditions :                                * */
/* *                                                                        * */
/* *   The above copyright notice and this permission notice shall be       * */
/* *   included in all copies or substantial portions of the Software.      * */
/* *                                                                        * */
/* *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,      * */
/* *   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF   * */
/* *   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND                * */
/* *   NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS   * */
/* *   BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN   * */
/* *   ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN    * */
/* *   CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE     * */
/* *   SOFTWARE.                                                            * */
/* *                                                                        * */
/* ************************************************************************** */
/******************************************************************************/

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
{}

DeferredLightingStage::~DeferredLightingStage()
{}

void DeferredLightingStage::SetDirLightShader(AssetHandle shader)
{
	dir_light_shader_ = shader;
}
void DeferredLightingStage::SetPointLightShader(AssetHandle shader)
{
	point_light_shader_ = shader;
}

void DeferredLightingStage::SetScene(Scene* scene)
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
	RenderingStage::Init();

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
	RenderingStage::Cleanup();
}

void DeferredLightingStage::DoRendering()
{
	if( !scene_ )
		return;

	fbo_->BeginRenderToTexture();

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	glBlendFunc(GL_ONE, GL_ONE);
	glEnable(GL_BLEND);

	BindInputTextures();

	ApplyDirectionalLights();
	ApplyPointLights();

	ReleaseInputTextures();

	glDisable(GL_BLEND);

	fbo_->EndRenderToTexture();
}

std::unique_ptr<DeferredLightingStage> DeferredLightingStage::CreateInstance(
	const xml::Element* properties, RenderingPipeline* pipline)
{
	auto asset_manager = pipline->GetAssetManager();
	auto dir_light_shader = 
		asset_manager->RequestAsset<ShaderProgram>(
		properties->FirstChildElement("DirectionalLightProgram"));
	auto point_light_shader =
		asset_manager->RequestAsset<ShaderProgram>(
		properties->FirstChildElement("PointLightShaderProgram"));

	auto stage = std::make_unique<DeferredLightingStage>(pipline);
	stage->SetDirLightShader(dir_light_shader);
	stage->SetPointLightShader(point_light_shader);

	std::string texture_name;
	if( xml::GetAttribute(properties, "Output", "texture", texture_name) )
		stage->SetColorOutputTexture(0, texture_name);

	std::vector<std::string> intput_textures;
	for( auto input : xml::IterateChildElements(properties, "Input") )
	{
		if( xml::GetAttribute(input, "texture", texture_name) )
			intput_textures.push_back(texture_name);
	}
	stage->SetInputTextures(intput_textures);

	return std::move(stage);
}

void DeferredLightingStage::ApplyDirectionalLights()
{
	Scene::ComponentMask mask =
		dir_light_container_->GetComponentMask();

	auto shader = GetAssetManager()->GetAsset<ShaderProgram>(dir_light_shader_);
	if( !shader ) return;

	shader->Use();

	glm::mat4 inv_projection = pipline_->GetCamera()->GetinverseProjectionMatrix();

	glUniformMatrix4fv(
		shader->GetUniformLocation("uInverseProjectionMatrix"),
		1, GL_FALSE, &inv_projection[0][0]);

	glm::mat3 normal_matrix = pipline_->GetCamera()->GetViewMatrix();

	for( const auto& it : dir_light_container_->GetComponents() )
	{
		glm::vec3 dir = it.second.direction;
		dir = normal_matrix*dir;

		glUniform3f(
			shader->GetUniformLocation("uLigthColor"),
			it.second.color.r,
			it.second.color.g,
			it.second.color.b);
		glUniform3f(
			shader->GetUniformLocation("uLigthDir"),
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
	auto shader = GetAssetManager()->GetAsset<ShaderProgram>(point_light_shader_);
	if( !shader ) return;

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


	shader->Use();

	glBindVertexArray(vao_);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_);

	glm::mat4 projection = camera->GetProjectionMatrix();
	glm::mat4 invers_projection = camera->GetinverseProjectionMatrix();

	glUniformMatrix4fv(
		shader->GetUniformLocation("uProjectionMatrix"),
		1, GL_FALSE, &projection[0][0]);
	glUniformMatrix4fv(
		shader->GetUniformLocation("uInverseProjectionMatrix"),
		1, GL_FALSE, &invers_projection[0][0]);
	glUniform2f(
		shader->GetUniformLocation("uTexelSize"),
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
