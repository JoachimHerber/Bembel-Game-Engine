#ifndef BEMBEL_DEFERREDLIGHTINGSTAGE_H
#define BEMBEL_DEFERREDLIGHTINGSTAGE_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include <BembelConfig.h>
#include <BembelOpenGL.h>

#include <BembelBase/XML.h>

#include "RenderingStage.h"
#include "LightSourceProperties.h"

#include <BembelKernel/Scene/PositionComponent.h>

#include <memory>
#include <array>

/*============================================================================*/
/* FORWARD DECLARATIONS                                                       */
/*============================================================================*/
namespace bembel{

class Scene;
class Texture;
class Shader;
class FrameBufferObject;

}//end of namespace bembel
 /*============================================================================*/
 /* CLASS DEFINITIONS                                                          */
 /*============================================================================*/
namespace bembel{

class BEMBEL_API DeferredLightingStage : public RenderingStage
{
public:
	using TexturePtr           = std::shared_ptr<Texture>;
	using ShaderProgramPtr     = std::shared_ptr<Shader>;
	using FrameBufferObjectPtr = std::shared_ptr<FrameBufferObject>;

	DeferredLightingStage(RenderingPipeline* pipline);
	~DeferredLightingStage();

	void SetDirLightShader(ShaderProgramPtr);
	void SetPointLightShader(ShaderProgramPtr);

	void SetOutputTexture(const std::string&);
	void SetInputTextures(const std::vector<std::string>&);

	virtual void SetScene(ScenePtr) override;

	virtual void Init() override;
	virtual void Cleanup() override;
	virtual void DoRendering() override;

	static std::unique_ptr<DeferredLightingStage>
		CreateInstance(const xml::Element*, RenderingPipeline*);

private:
	static ShaderProgramPtr CreateShader(const xml::Element*);

	void SetTextureSamplerUniforms(Shader* shader);

	void BindTextures();
	void ReleaseTextures();

	void ApplyDirectionalLights();
	void ApplyPointLights();

private:
	FrameBufferObjectPtr _fbo;

	ShaderProgramPtr  _dirLightShader;
	ShaderProgramPtr  _pointLightShader;

	GLuint _vao;
	GLuint _vbo;
	unsigned _bufferSize;

	std::vector<TexturePtr>  _inputTextures;
	std::vector<std::string> _inputTexturNames;

	ScenePtr                           _scene;
	DirLightProperties::ContainerPtr   _dirLightContainer;
	PointLightProperties::ContainerPtr _pointLightContainer;
	PositionComponent::ContainerPtr    _positionConteiner;
};

} //end of namespace bembel
  /*============================================================================*/
  /* END OF FILE                                                                */
  /*============================================================================*/
#endif //include guards