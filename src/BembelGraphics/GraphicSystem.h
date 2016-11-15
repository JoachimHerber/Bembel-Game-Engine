#ifndef BEMBEL_GRAPHICSSYSTEM_H
#define BEMBEL_GRAPHICSSYSTEM_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include <BembelConfig.h>

#include <BembelKernel/System.h>
#include <BembelKernel/Events/DisplayEvents.h>

#include <BembelBase/Factory.hpp>

#include "RenderingPipeline/RenderingStage.h"

#include <vector>

/*============================================================================*/
/* FORWARD DECLARATIONS                                                       */
/*============================================================================*/
namespace bembel{

class Viewport;
class RenderingPipeline;

}//end of namespace bembel
/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel {

class BEMBEL_API GraphicSystem : public System
{
public:
	using ViewportPtr            = std::shared_ptr<Viewport>;
	using RenderingPipelinePtr   = std::shared_ptr<RenderingPipeline>;
	using RendertingStageFactory = TFactory<RenderingStage, const xml::Element*, RenderingPipeline*>;

	GraphicSystem(Kernel*);
	~GraphicSystem();

	std::shared_ptr<Viewport> CreateViewPort(unsigned windowID = 0);
	std::vector<std::shared_ptr<Viewport>>& GetViewports(unsigned windowID = 0);
	void UpdateViewports();

	RenderingPipelinePtr CreateRenderingPipline();
	std::vector<RenderingPipelinePtr>& GetRenderingPiplies();

	RendertingStageFactory& GetRendertingStageFactory();

	virtual bool Configure(const xml::Element*) override;

	virtual bool Init() override;
	virtual void Shutdown() override;
	virtual void Update(double timeSinceLastUpdate) override;

	void HandleEvent(const WindowUpdateEvent&);
	void HandleEvent(const FrameBufferResizeEvent&);

private:
	void ConfigurePipelines(const xml::Element*);
	void ConfigureViewports(const xml::Element*);

private:
	std::vector<std::vector<ViewportPtr>> _viewports;
	std::vector<RenderingPipelinePtr>     _pipelines;

	RendertingStageFactory _renderingStageFactory;
};

struct InitGraphicResourcesEvent
{
	BEMBEL_EVENT_INTERVACE_DECLADRATION
};

struct CleanuptGraphicResourcesEvent
{
	BEMBEL_EVENT_INTERVACE_DECLADRATION
};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
