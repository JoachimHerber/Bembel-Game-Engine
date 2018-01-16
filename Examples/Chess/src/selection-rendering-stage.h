/******************************************************************************/
/* ************************************************************************** */
/* *                                                                        * */
/* *    MIT License                                                         * */
/* *                                                                        * */
/* *   Copyright(c) 2018 Joachim Herber                                     * */
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

#ifndef BEMBEL_SELECTIONRENDERINGSTAGE_H
#define BEMBEL_SELECTIONRENDERINGSTAGE_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include <bembel-open-gl.h>

#include "selection-component.h"

#include <bembel-base/xml.h>
#include <bembel-kernel/scene/position-component.h>
#include <bembel-kernel/scene/rotation-component.h>
#include <bembel-graphics/graphic-system.h>
#include <bembel-graphics/geometry/geometry-component.h>
#include <bembel-graphics/rendering-pipeline/rendering-stage.h>

#include <chrono>

#include <memory>
#include <vector>

/*============================================================================*/
/* FORWARD DECLARATIONS                                                       */
/*============================================================================*/
namespace bembel{

class ShaderProgram;
class Texture;
class FrameBufferObject;
class SelectionManager;
class GeometryModel;

}//end of namespace bembel
/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
class SelectionRenderingStage : public bembel::RenderingStage
{
public:
	SelectionRenderingStage( bembel::RenderingPipeline* pipline);
	~SelectionRenderingStage();

	void SetShader(bembel::AssetHandle);

	void SetDepthOutputTexture(const std::string&);
	void SetColorOutputTexture(const std::string&);

	virtual void Init() override;
	virtual void Cleanup() override;
	virtual void DoRendering() override;

	void SetScene(bembel::Scene*) override;

	static std::unique_ptr<SelectionRenderingStage>
		CreateInstance(const bembel::xml::Element*, bembel::RenderingPipeline*);

private:
	struct GeometryObject
	{
		glm::vec3              position;
		glm::quat              rotation;
		float                  dist;
		bembel::GeometryModel* model;
		unsigned               state;
	};
	void GetHiglightedObjects(std::vector<GeometryObject>& );

private:
	bembel::AssetHandle shader_;

	std::unique_ptr<bembel::Texture> noise_;

	bembel::Scene*                             scene_;
	bembel::PositionComponent::ContainerType*  position_components_;
	bembel::RotationComponent::ContainerType*  rotation_components_;
	bembel::GeometryComponent::ContainerType*  geometry_components_;
	SelectionComponent::ContainerType*         selection_components_;

	std::chrono::time_point<std::chrono::steady_clock> start_time_;
};
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
