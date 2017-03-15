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
namespace bembel{

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
	using TexturePtr = std::shared_ptr<Texture>;

	GeometryRenderingStage(RenderingPipeline* pipline);
	~GeometryRenderingStage();

	void SetDepthOutputTexture(const std::string&);
	void SetColorOutputTexture(unsigned index, const std::string&);

	virtual void Init() override;
	virtual void Cleanup() override;
	virtual void DoRendering() override;

	void SetScene(ScenePtr) override;

	static std::unique_ptr<GeometryRenderingStage>
		CreateInstance(const xml::Element*, RenderingPipeline*);

private:
	std::unique_ptr<FrameBufferObject> _fbo;

	ScenePtr                          _scene;
	GeometryComponent::ContainerType* _geometryComponents;
	PositionComponent::ContainerType* _positionComponents;
};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
