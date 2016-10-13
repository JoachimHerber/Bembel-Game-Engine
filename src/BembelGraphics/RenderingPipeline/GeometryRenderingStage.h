#ifndef BEMBEL_GEOMETRYRENDERINGSTAGE_H
#define BEMBEL_GEOMETRYRENDERINGSTAGE_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include <BembelConfig.h>
#include <BembelOpenGL.h>

#include <BembelBase/XML.h>
#include <BembelKernel/Scene/PositionComponent.h>
#include <BembelKernel/Scene/GeometryComponent.h>

#include "RenderingStage.h"

#include <memory>
#include <vector>

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
	std::shared_ptr<FrameBufferObject> _fbo;

	ScenePtr                        _scene;
	GeometryComponent::ContainerPtr _geometryComponents;
	PositionComponent::ContainerPtr _positionComponents;

};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
