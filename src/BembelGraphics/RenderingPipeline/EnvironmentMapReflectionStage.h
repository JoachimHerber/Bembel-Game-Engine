#ifndef BEMBEL_ENVIRONMENTMAPREFLECTIONSTAGE_H
#define BEMBEL_ENVIRONMENTMAPREFLECTIONSTAGE_H
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

class Shader;
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
	using TexturePtr = std::shared_ptr<Texture>;
	using ShaderPtr  = std::shared_ptr<Shader>;

	EnvironmentMapReflectionStage(RenderingPipeline* pipline);
	~EnvironmentMapReflectionStage();

	bool InitEnvironmentMap(
		const std::string& left,
		const std::string& right,
		const std::string& bottom,
		const std::string& top,
		const std::string& back,
		const std::string& front);

	void SetShader(ShaderPtr);

	void SetOutputTexture(const std::string&);
	void SetInputTextures(const std::vector<std::string>&);

	virtual void Init() override;
	virtual void Cleanup() override;
	virtual void DoRendering() override;

	static std::unique_ptr<EnvironmentMapReflectionStage>
		CreateInstance(const xml::Element*, RenderingPipeline*);

private:
	static ShaderPtr CreateShader(const xml::Element*);
	void SetTextureSamplerUniforms(Shader* shader);

	void BindTextures();
	void ReleaseTextures();

private:
	std::unique_ptr<FrameBufferObject> _fbo;

	TexturePtr _environmentMap;
	ShaderPtr  _shader;

	std::vector<TexturePtr>  _inputTextures;
	std::vector<std::string> _inputTexturNames;
};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
