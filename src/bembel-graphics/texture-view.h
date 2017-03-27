#ifndef BEMBEL_GRAPHICS_RENDERINGPIPELINEVIEW_H_
#define BEMBEL_GRAPHICS_RENDERINGPIPELINEVIEW_H_
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "bembel-config.h"
#include "viewport.h"

#include <glm/glm.hpp>
/*============================================================================*/
/* FORWARD DECLARATIONS                                                       */
/*============================================================================*/
namespace bembel {

class Texture;
class AssetManager;
class ShaderProgram;

}//end of namespace bembel
/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel {

class BEMBEL_API TextureView : public Viewport::View
{
public:
	TextureView(AssetManager*, Texture* color);
	~TextureView();

	const glm::vec2& GetViewAreaMin() const;
	const glm::vec2& GetViewAreaMax() const;

	void SetViewArea(const glm::vec2& min, const glm::vec2& max);

	void Init();
	void Cleanup();

	virtual void Draw() override;

private:
	AssetManager* asset_manager_;
	Texture* texture_;

	glm::vec2 min_;
	glm::vec2 max_;

	ShaderProgram* shader_;
	int            uniform_;
};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
