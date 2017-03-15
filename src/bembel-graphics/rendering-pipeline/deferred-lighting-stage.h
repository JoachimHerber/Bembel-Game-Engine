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
	using FrameBufferObjectPtr = std::unique_ptr<FrameBufferObject>;

	DeferredLightingStage(RenderingPipeline* pipline);
	~DeferredLightingStage();

	void SetDirLightShader(ShaderProgramPtr);
	void SetPointLightShader(ShaderProgramPtr);

	bool InitShader(
		const std::string& pointLightVert,
		const std::string& pointLightFrag,
		const std::string& dirLightVert,
		const std::string& dirLightFrag );

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

	ScenePtr                             _scene;
	DirLightProperties::ContainerType*   _dirLightContainer;
	PointLightProperties::ContainerType* _pointLightContainer;
	PositionComponent::ContainerType*    _positionContainer;
};

} //end of namespace bembel
  /*============================================================================*/
  /* END OF FILE                                                                */
  /*============================================================================*/
#endif //include guards