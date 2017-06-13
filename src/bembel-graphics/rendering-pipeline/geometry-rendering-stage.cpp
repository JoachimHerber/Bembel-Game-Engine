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

#include "geometry-rendering-stage.h"

#include "camera.h"
#include "rendering-pipeline.h"
#include "../graphic-system.h"
#include "../geometry/geometry-renderer.h"

#include <glm/gtc/matrix_transform.hpp>

#include <bembel-kernel/rendering/texture.h>
#include <bembel-kernel/rendering/frame-buffer-object.h>

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel {

GeometryRenderingStage::GeometryRenderingStage(RenderingPipeline* pipline)
	: RenderingStage(pipline)
{}

GeometryRenderingStage::~GeometryRenderingStage()
{}

void GeometryRenderingStage::DoRendering()
{
	if( !scene_ )
		return;

	fbo_->BeginRenderToTexture();

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	glClearColor(0.0, 0.0, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	auto cam = pipline_->GetCamera();

	const auto& entitis = scene_->GetEntitys();
	const std::vector<PositionComponent>& position_components = position_components_->GetComponents();
	const std::vector<RotationComponent>& rotation_components = rotation_components_->GetComponents();
	const std::vector<GeometryComponent>& geometry_components = geometry_components_->GetComponents();

	auto& renderQueue = pipline_->GetGraphicSystem()->GetGeometryRenderQueue();
	renderQueue.ClearRendarData();

	for( Scene::EntityID entity = 0; entity < entitis.size(); ++entity )
	{
		if( (entitis[entity] & geometry_components_->GetComponentMask()) == 0 )
			continue;

		glm::mat4 transform(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1);
		if( entitis[entity] & position_components_->GetComponentMask() )
		{
			transform = glm::translate(
				transform, position_components[entity].position);
		}
		if( entitis[entity] & rotation_components_->GetComponentMask() )
		{
			transform = transform*glm::mat4_cast(rotation_components[entity].rotation);
		}

		const GeometryComponent& geom = geometry_components[entity];
		renderQueue.AddGeometryObject(geom.model, transform);
	}

	renderQueue.SortRenderData();
	for( auto& renderer :pipline_->GetGraphicSystem()->GetRenderer() )
	{
		renderer->Render(
			cam->GetProjectionMatrix(),
			cam->GetViewMatrix(),
			renderQueue.GetRenderData());
	}
	fbo_->EndRenderToTexture();
}

void GeometryRenderingStage::SetScene(Scene* scene)
{
	scene_ = scene;

	geometry_components_ = scene->RequestComponentContainer<GeometryComponent>();
	position_components_ = scene->RequestComponentContainer<PositionComponent>();
	rotation_components_ = scene->RequestComponentContainer<RotationComponent>();
}

std::unique_ptr<GeometryRenderingStage> GeometryRenderingStage::CreateInstance(
	const xml::Element* properties,
	RenderingPipeline* pipline)
{
	auto stage = std::make_unique<GeometryRenderingStage>(pipline);

	std::string texture_name;
	if( xml::GetAttribute(properties, "DepthOutput", "texture", texture_name) )
		stage->SetDepthOutputTexture(texture_name);

	for( auto colorOutput : xml::IterateChildElements(properties, "ColorOutput") )
	{
		unsigned attachment;
		if( xml::GetAttribute(colorOutput, "texture", texture_name) &&
			xml::GetAttribute(colorOutput, "attachment", attachment) )
		{
			stage->SetColorOutputTexture(attachment, texture_name);
		}
	}

	return std::move(stage);
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
