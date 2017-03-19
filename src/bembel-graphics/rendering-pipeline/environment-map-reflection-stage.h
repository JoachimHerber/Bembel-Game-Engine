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

/*============================================================================*/
/* FORWARD DECLARATIONS                                                       */
/*============================================================================*/
namespace bembel {

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
	using ShaderPtr = std::shared_ptr<Shader>;

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
	std::unique_ptr<FrameBufferObject> fbo_;

	TexturePtr environment_map_;
	ShaderPtr  shader_;

	std::vector<TexturePtr>  input_textures_;
	std::vector<std::string> input_textur_names_;
};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
