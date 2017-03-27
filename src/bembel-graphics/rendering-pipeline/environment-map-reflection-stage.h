#ifndef BEMBEL_GRAPHICS_RENDERING_PIPELINE_ENVIRONMENTMAPREFLECTIONSTAGE_H_
#define BEMBEL_GRAPHICS_RENDERING_PIPELINE_ENVIRONMENTMAPREFLECTIONSTAGE_H_
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "bembel-config.h"
#include "bembel-open-gl.h"

#include "rendering-stage.h"

#include <memory>
#include <vector>

#include <bembel-base/xml.h>
#include <bembel-kernel/assets/asset-handle.h>

/*============================================================================*/
/* FORWARD DECLARATIONS                                                       */
/*============================================================================*/
namespace bembel {

class ShaderProgram;
class Texture;
class FrameBufferObject;

}//end of namespace bembel
/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel {

class BEMBEL_API EnvironmentMapReflectionStage : public RenderingStage
{
public:
	EnvironmentMapReflectionStage(RenderingPipeline* pipline);
	~EnvironmentMapReflectionStage();

	bool InitEnvironmentMap(
		const std::string& left,
		const std::string& right,
		const std::string& bottom,
		const std::string& top,
		const std::string& back,
		const std::string& front);

	void SetShader(AssetHandle);

	void SetOutputTexture(const std::string&);

	virtual void Init() override;
	virtual void Cleanup() override;
	virtual void DoRendering() override;

	static std::unique_ptr<EnvironmentMapReflectionStage>
		CreateInstance(const xml::Element*, RenderingPipeline*);

private:
	std::unique_ptr<Texture> environment_map_;

	AssetHandle  shader_program_;
};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
