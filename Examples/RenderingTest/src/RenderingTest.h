#ifndef BEMBEL_RENDERINGTEST_H
#define BEMBEL_RENDERINGTEST_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/
#include <BembelKernel/Application.h>
#include <BembelKernel/Events/EventManager.h>
#include <BembelGraphics/GraphicSystem.h>
#include <BembelGraphics/RenderingPipeline/RenderingPipeline.h>

#include "CameraControle.h"
/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel{

class RenderingTest : public Application
{
public:
	RenderingTest();
	~RenderingTest();

	virtual bool Init() override;
	virtual void Cleanup() override;

	virtual void Update(double time) override;

	void HandleEvent(const WindowShouldCloseEvent&);
	void HandleEvent(const FrameBufferResizeEvent&);

private:
	std::shared_ptr<GraphicSystem>  _graphicSys;
	std::shared_ptr<CameraControle> _cam;
};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
