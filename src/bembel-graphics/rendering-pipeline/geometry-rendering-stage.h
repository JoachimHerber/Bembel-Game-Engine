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
};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
