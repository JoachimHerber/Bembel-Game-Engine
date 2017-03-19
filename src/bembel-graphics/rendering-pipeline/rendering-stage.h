#ifndef BEMBEL_GRAPHICS_RENDERING_PIPELINE_RENDERINGSTAGE_H_
#define BEMBEL_GRAPHICS_RENDERING_PIPELINE_RENDERINGSTAGE_H_
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "bembel-config.h"

#include <memory>

/*============================================================================*/
/* FORWARD DECLARATIONS                                                       */
/*============================================================================*/
namespace bembel{

class RenderingPipeline;
class Scene;

}//end of namespace bembel
/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel {

class BEMBEL_API RenderingStage
{
public:
	RenderingStage(RenderingPipeline* pipline)
		: pipline_(pipline)
	{}
	virtual ~RenderingStage()
	{}

	using ScenePtr = std::shared_ptr<Scene>;
	virtual void SetScene(ScenePtr)
	{}

	virtual void Init() = 0;
	virtual void Cleanup() = 0;

	virtual void DoRendering() = 0;

protected:
	RenderingPipeline* pipline_;
};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
