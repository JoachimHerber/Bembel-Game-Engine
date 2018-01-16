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

#ifndef BEMBEL_GRAPHICS_RENDERING_PIPELINE_DEFERREDLIGHTINGSTAGE_H_
#define BEMBEL_GRAPHICS_RENDERING_PIPELINE_DEFERREDLIGHTINGSTAGE_H_
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "bembel-config.h"
#include "bembel-open-gl.h"

#include "rendering-stage.h"
#include "light-source-component.h"

#include <memory>

#include <bembel-base/xml.h>
#include <bembel-kernel/scene/position-component.h>

/*============================================================================*/
/* FORWARD DECLARATIONS                                                       */
/*============================================================================*/
namespace bembel {

class Scene;
class Texture;
class ShaderProgram;
class FrameBufferObject;

}//end of namespace bembel
/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel {

class BEMBEL_API DeferredLightingStage : public RenderingStage
{
public:
	DeferredLightingStage(RenderingPipeline* pipline);
	~DeferredLightingStage();

	void SetDirLightShader(AssetHandle);
	void SetPointLightShader(AssetHandle);

	virtual void SetScene(Scene*) override;

	virtual void Init() override;
	virtual void Cleanup() override;
	virtual void DoRendering() override;

	static std::unique_ptr<DeferredLightingStage>
		CreateInstance(const xml::Element*, RenderingPipeline*);

private:
	void ApplyDirectionalLights();
	void ApplyPointLights();

private:
	AssetHandle dir_light_shader_;
	AssetHandle point_light_shader_;

	GLuint vao_;
	GLuint vbo_;
	unsigned buffer_size_;

	Scene*                               scene_ = nullptr;
	DirLightProperties::ContainerType*   dir_light_container_ = nullptr;
	PointLightProperties::ContainerType* point_light_container_ = nullptr;
	PositionComponent::ContainerType*    position_container_ = nullptr;
};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards