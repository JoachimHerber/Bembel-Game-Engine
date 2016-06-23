#ifndef BEMBEL_DEFERREDRENDERINGPIPLINE_H
#define BEMBEL_DEFERREDRENDERINGPIPLINE_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include <BembelGraphics/RenderingPipeline.h>
#include <BembelGraphics/RenderingStage.h>
#include "SimpleGeometryRenderer.h"

/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel {

class TestRenderingPipline : public RenderingPipeline
{
public:
	TestRenderingPipline();
	~TestRenderingPipline();

	TestRenderer* GetRenderer();

	Texture* GetNormalBuffer();
	Texture* GetDiffuseBuffer();
	Texture* GetSpecularBuffer();

	virtual bool Init() override;
	virtual bool Cleanup() override;

private:
	class GeometryStage : public RenderingStage
	{
	public:
		GeometryStage(TestRenderingPipline*);
		~GeometryStage();

		virtual void DoRendering(RenderingPipeline*) override;

	private:
		TestRenderingPipline* _pipline;
	};

	class LightingStage : public RenderingStage
	{
	public:
		LightingStage(TestRenderingPipline*);
		~LightingStage();

		virtual void DoRendering(RenderingPipeline*) override;

	private:
		TestRenderingPipline* _pipline;
	};
private:
	std::unique_ptr<TestRenderer> _renderer;

	unsigned _normalBuffer;
	unsigned _diffuseBuffer;
	unsigned _specularBuffer;
};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
