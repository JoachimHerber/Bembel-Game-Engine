#ifndef BEMBEL_RENDERINGPIPELINEVIEW_H
#define BEMBEL_RENDERINGPIPELINEVIEW_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include <BembelConfig.h>
#include "Viewport.h"


#include <glm/glm.hpp>
/*============================================================================*/
/* FORWARD DECLARATIONS                                                       */
/*============================================================================*/
namespace bembel{

class Shader;
class Texture;

}//end of namespace bembel
/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel {

class BEMBEL_API TextureView : public Viewport::View
{
public:
	using ShaderPtr  = std::shared_ptr<Shader>;
	using TexturePtr = std::shared_ptr<Texture>;

	TextureView(TexturePtr color);
	~TextureView();

	void SetViewArea(const glm::vec2& min, const glm::vec2& max);

	void Init();
	void Cleanup();

	virtual void Draw() override;

private:
	TexturePtr _color;

	glm::vec2 _min;
	glm::vec2 _max;

	ShaderPtr _shader;
	int       _uniform;
};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
