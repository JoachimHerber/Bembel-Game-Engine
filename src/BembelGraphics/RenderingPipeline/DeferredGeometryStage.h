#ifndef BEMBEL_DEFERREDGEOMETRYSTAGE_H
#define BEMBEL_DEFERREDGEOMETRYSTAGE_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include <BembelConfig.h>
#include <BembelOpenGL.h>

#include <BembelBase/XML.h>
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

class BEMBEL_API DeferredGeometryStage : public RenderingStage
{
public:
	using TexturePtr = std::shared_ptr<Texture>;

	DeferredGeometryStage(RenderingPipeline* pipline);
	~DeferredGeometryStage();

	void SetDepthOutputTexture(const std::string&);
	void SetColorOutputTexture(unsigned index, const std::string&);

	virtual void Init() override;
	virtual void Cleanup() override;
	virtual void DoRendering() override;

	static std::unique_ptr<DeferredGeometryStage>
		CreateInstance(const xml::Element*, RenderingPipeline*);

private:
	std::shared_ptr<FrameBufferObject> _fbo;
};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
