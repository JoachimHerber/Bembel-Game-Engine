#ifndef BEMBEL_GEOMETRYRENDERER_H
#define BEMBEL_GEOMETRYRENDERER_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include <BembelConfig.h>
#include <BembelOpenGL.h>

#include "GeometryInstance.h"
#include "../RenderingPipeline/Renderer.h"

#include <BembelKernel/Scene/PositionComponent.h>
#include <BembelKernel/Assets/AssetContainer.h>
#include <BembelKernel/Assets/AssetHandle.h>

#include <memory>
#include <map>

/*============================================================================*/
/* FORWARD DECLARATIONS                                                       */
/*============================================================================*/
namespace bembel{

class Shader;
class GeometryModel;
class Material;
class Texture;

}//end of namespace bembel
/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel{

class BEMBEL_API GeometryRenderer : public Renderer
{
public:
	GeometryRenderer();
	~GeometryRenderer();

	void Init() override;
	void Cleanup() override;

	virtual void SetScene(ScenePtr) override;

	void DoGeometryPass(
		const glm::mat4& view,
		const glm::mat4& projection,
		const ViewFrustum& frustum
	) override;
	void DoShadowPass(
		const glm::mat4& view, 
		const glm::mat4& projection,
		const ViewFrustum& frustum
	) override;

	static std::unique_ptr<GeometryRenderer> CreateInstance(const xml::Element*);

private:
	void DrawGeometry(
		const glm::mat4& view,
		const glm::mat4& projection,
		const ViewFrustum& frustum,
		bool shadowPass);

	using VisibleGeometry = std::map<AssetHandle, std::vector<glm::mat4>>;
	void GetEntitysWhithinViewFrustum(
		const glm::mat4& view,
		const ViewFrustum& frustum, 
		VisibleGeometry& geometry,
		bool);


	bool UseMaterial(AssetHandle materialHandle);
	void ApplyTexture(GLenum textureUnit, AssetHandle textureHandle, const std::string& uniform);

private:
	std::unique_ptr<Shader> _geometryPassShader;
	std::unique_ptr<Shader> _shadowPassShader;

	ScenePtr                        _scene;
	GeometryInstance::ContainerPtr  _geometryContainer;
	PositionComponent::ContainerPtr _positionConteiner;
};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
