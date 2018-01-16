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

#ifndef BEMBEL_GRAPHICS_GRAPHICSSYSTEM_H_
#define BEMBEL_GRAPHICS_GRAPHICSSYSTEM_H_
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "bembel-config.h"

#include "geometry/geometry-render-queue.h"
#include "rendering-pipeline/rendering-stage.h"

#include <vector>

#include <bembel-base/factory.hpp>
#include <bembel-kernel/system.h>
#include <bembel-kernel/events/display-events.h>

/*============================================================================*/
/* FORWARD DECLARATIONS                                                       */
/*============================================================================*/
namespace bembel {

class Viewport;
class RenderingPipeline;
class GeometryRendererBase;

}//end of namespace bembel
/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel {

class BEMBEL_API GraphicSystem : public System
{
public:
	using ViewportPtr = std::shared_ptr<Viewport>;
	using RendererPtr = std::shared_ptr<GeometryRendererBase>;
	using RenderingPipelinePtr = std::shared_ptr<RenderingPipeline>;
	using RendertingStageFactory = TFactory<RenderingStage, const xml::Element*, RenderingPipeline*>;

	GraphicSystem(Kernel*);
	GraphicSystem(const GraphicSystem&) = delete;
	GraphicSystem& operator=(const GraphicSystem&) = delete;
	~GraphicSystem();

	Viewport*                       CreateViewPort(unsigned windowID = 0);
	const std::vector<ViewportPtr>& GetViewports(unsigned windowID = 0);
	void UpdateViewports();

	const std::vector<RendererPtr>& GetRenderer() const;
	GeometryRendererBase* GetRenderer(const std::string& name) const;

	RenderingPipeline* CreateRenderingPipline();
	const std::vector<RenderingPipelinePtr>& GetRenderingPiplies();

	RendertingStageFactory& GetRendertingStageFactory();
	GeometryRenderQueue&    GetGeometryRenderQueue();

	virtual bool Configure(const xml::Element*) override;

	virtual bool Init() override;
	virtual void Shutdown() override;
	virtual void Update(double time_since_last_update) override;

	void HandleEvent(const WindowUpdateEvent&);
	void HandleEvent(const FrameBufferResizeEvent&);

private:
	void ConfigureRenderer(const xml::Element*);
	void ConfigurePipelines(const xml::Element*);
	void ConfigureViewports(const xml::Element*);

private:
	std::vector<std::vector<ViewportPtr>> viewports_;
	std::vector<RendererPtr>              renderer_;
	std::vector<RenderingPipelinePtr>     pipelines_;

	std::map<std::string, size_t> renderer_map_;

	RendertingStageFactory rendering_stage_factory_;
	GeometryRenderQueue    geometry_render_queue_;
};

struct InitGraphicResourcesEvent
{};

struct CleanuptGraphicResourcesEvent
{};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
