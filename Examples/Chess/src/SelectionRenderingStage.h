#ifndef BEMBEL_SELECTIONRENDERINGSTAGE_H
#define BEMBEL_SELECTIONRENDERINGSTAGE_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include <BembelOpenGL.h>

#include "SelectionComponent.h"

#include <BembelBase/XML.h>
#include <BembelKernel/Scene/PositionComponent.h>
#include <BembelGraphics/GraphicSystem.h>
#include <BembelGraphics/Geometry/GeometryComponent.h>
#include <BembelGraphics/RenderingPipeline/RenderingStage.h>

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
namespace bembel {

class SelectionRenderingStage : public RenderingStage
{
public:
	using TexturePtr = std::shared_ptr<Texture>;
	using ShaderPtr  = std::shared_ptr<Shader>;

	SelectionRenderingStage(RenderingPipeline* pipline);
	~SelectionRenderingStage();

	void SetShader(ShaderPtr);

	void SetDepthOutputTexture(const std::string&);
	void SetColorOutputTexture(const std::string&);

	virtual void Init() override;
	virtual void Cleanup() override;
	virtual void DoRendering() override;

	void SetScene(ScenePtr) override;

	static std::unique_ptr<SelectionRenderingStage>
		CreateInstance(const xml::Element*, RenderingPipeline*);

private:
	static ShaderPtr CreateShader(const xml::Element*);


	struct GeometryObject
	{
		glm::vec3      position;
		float          dist;
		GeometryModel* model;
		unsigned       state;
	};
	void GetHiglightedObjects(std::vector<GeometryObject>& );

private:
	std::unique_ptr<FrameBufferObject> _fbo;

	ShaderPtr  _shader;

	ScenePtr                         _scene;
	PositionComponent::ContainerPtr  _positionComponents;
	SelectionComponent::ContainerPtr _selectionComponents;
	GeometryComponent::ContainerPtr  _geometryComponents;
};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
