#ifndef BEMBEL_SELECTIONRENDERINGSTAGE_H
#define BEMBEL_SELECTIONRENDERINGSTAGE_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include <bembel-open-gl.h>

#include "selection-component.h"

#include <bembel-base/xml.h>
#include <bembel-kernel/scene/position-component.h>
#include <bembel-graphics/graphic-system.h>
#include <bembel-graphics/geometry/geometry-component.h>
#include <bembel-graphics/rendering-pipeline/rendering-stage.h>

#include <chrono>

#include <memory>
#include <vector>

/*============================================================================*/
/* FORWARD DECLARATIONS                                                       */
/*============================================================================*/
namespace bembel{

class Shader;
class Texture;
class FrameBufferObject;
class SelectionManager;
class GeometryModel;

}//end of namespace bembel
/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
class SelectionRenderingStage : public bembel::RenderingStage
{
public:
	using TexturePtr = std::shared_ptr<bembel::Texture>;
	using ShaderPtr  = std::shared_ptr<bembel::Shader>;

	SelectionRenderingStage( bembel::RenderingPipeline* pipline);
	~SelectionRenderingStage();

	void SetShader(ShaderPtr);

	void SetDepthOutputTexture(const std::string&);
	void SetColorOutputTexture(const std::string&);

	virtual void Init() override;
	virtual void Cleanup() override;
	virtual void DoRendering() override;

	void SetScene(ScenePtr) override;

	static std::unique_ptr<SelectionRenderingStage>
		CreateInstance(const bembel::xml::Element*, bembel::RenderingPipeline*);

private:
	static ShaderPtr CreateShader(const bembel::xml::Element*);

	struct GeometryObject
	{
		glm::vec3              position;
		float                  dist;
		bembel::GeometryModel* model;
		unsigned               state;
	};
	void GetHiglightedObjects(std::vector<GeometryObject>& );

private:
	std::unique_ptr<bembel::FrameBufferObject> _fbo;

	ShaderPtr  _shader;
	TexturePtr _noise;

	ScenePtr                                 _scene;
	bembel::PositionComponent::ContainerPtr  _positionComponents;
	SelectionComponent::ContainerPtr         _selectionComponents;
	bembel::GeometryComponent::ContainerPtr  _geometryComponents;

	std::chrono::time_point<std::chrono::steady_clock> _starTime;
};
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
