#ifndef BEMBEL_GRAPHICS_RENDERING_PIPELINE_RENDERINGSTAGE_H_
#define BEMBEL_GRAPHICS_RENDERING_PIPELINE_RENDERINGSTAGE_H_
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "bembel-config.h"

#include <memory>
#include <vector>

/*============================================================================*/
/* FORWARD DECLARATIONS                                                       */
/*============================================================================*/
namespace bembel {

class RenderingPipeline;
class AssetManager;
class Scene;
class Texture;
class FrameBufferObject;

}//end of namespace bembel
/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel {

class BEMBEL_API RenderingStage
{
public:
	RenderingStage(RenderingPipeline* pipline);
	virtual ~RenderingStage();

	virtual void SetScene(Scene*);

	virtual void Init();
	virtual void Cleanup();

	virtual void DoRendering() = 0;

protected:
	AssetManager* GetAssetManager() const;

	void SetInputTextures(const std::vector<std::string>&);

	void SetDepthOutputTexture(const std::string&);
	void SetColorOutputTexture(unsigned index, const std::string&);

	void BindInputTextures();
	void ReleaseInputTextures();

protected:
	RenderingPipeline* pipline_;

	std::vector<Texture*>              textures_;
	std::unique_ptr<FrameBufferObject> fbo_;

};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
