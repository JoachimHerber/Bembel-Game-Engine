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

#ifndef BEMBEL_GRAPHICS_RENDERING_PIPELINE_GEOMETRYRENDERINGSTAGE_H_
#define BEMBEL_GRAPHICS_RENDERING_PIPELINE_GEOMETRYRENDERINGSTAGE_H_
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "bembel-config.h"
#include "bembel-open-gl.h"

#include "rendering-stage.h"
#include "../geometry/geometry-component.h"

#include <memory>
#include <vector>

#include <bembel-base/xml.h>
#include <bembel-kernel/scene/position-component.h>
#include <bembel-kernel/scene/rotation-component.h>

/*============================================================================*/
/* FORWARD DECLARATIONS                                                       */
/*============================================================================*/
namespace bembel {

class Texture;
class FrameBufferObject;

}//end of namespace bembel
/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel {

class BEMBEL_API GeometryRenderingStage : public RenderingStage
{
public:
	GeometryRenderingStage(RenderingPipeline* pipline);
	~GeometryRenderingStage();

	virtual void DoRendering() override;

	void SetScene(Scene*) override;

	static std::unique_ptr<GeometryRenderingStage>
		CreateInstance(const xml::Element*, RenderingPipeline*);

private:
	Scene*                            scene_ = nullptr;
	GeometryComponent::ContainerType* geometry_components_ = nullptr;
	PositionComponent::ContainerType* position_components_ = nullptr;
	RotationComponent::ContainerType* rotation_components_ = nullptr;
};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
