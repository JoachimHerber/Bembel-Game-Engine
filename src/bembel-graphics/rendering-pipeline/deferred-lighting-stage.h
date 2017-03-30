#ifndef BEMBEL_GRAPHICS_RENDERING_PIPELINE_DEFERREDLIGHTINGSTAGE_H_
#define BEMBEL_GRAPHICS_RENDERING_PIPELINE_DEFERREDLIGHTINGSTAGE_H_
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "bembel-config.h"
#include "bembel-open-gl.h"

#include "rendering-stage.h"
#include "light-source-component.h"

#include <memory>

#include <bembel-base/xml.h>
#include <bembel-kernel/scene/position-component.h>

/*============================================================================*/
/* FORWARD DECLARATIONS                                                       */
/*============================================================================*/
namespace bembel {

class Scene;
class Texture;
class ShaderProgram;
class FrameBufferObject;

}//end of namespace bembel
/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel {

class BEMBEL_API DeferredLightingStage : public RenderingStage
{
public:
	DeferredLightingStage(RenderingPipeline* pipline);
	~DeferredLightingStage();

	void SetDirLightShader(AssetHandle);
	void SetPointLightShader(AssetHandle);

	virtual void SetScene(Scene*) override;

	virtual void Init() override;
	virtual void Cleanup() override;
	virtual void DoRendering() override;

	static std::unique_ptr<DeferredLightingStage>
		CreateInstance(const xml::Element*, RenderingPipeline*);

private:
	void ApplyDirectionalLights();
	void ApplyPointLights();

private:
	AssetHandle dir_light_shader_;
	AssetHandle point_light_shader_;

	GLuint vao_;
	GLuint vbo_;
	unsigned buffer_size_;

	Scene*                               scene_ = nullptr;
	DirLightProperties::ContainerType*   dir_light_container_ = nullptr;
	PointLightProperties::ContainerType* point_light_container_ = nullptr;
	PositionComponent::ContainerType*    position_container_ = nullptr;
};

} //end of namespace bembel
  /*============================================================================*/
  /* END OF FILE                                                                */
  /*============================================================================*/
#endif //include guards