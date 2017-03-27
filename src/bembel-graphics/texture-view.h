#ifndef BEMBEL_GRAPHICS_RENDERINGPIPELINEVIEW_H_
#define BEMBEL_GRAPHICS_RENDERINGPIPELINEVIEW_H_
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "bembel-config.h"
#include "bembel-open-gl.h"
#include "viewport.h"

#include <glm/glm.hpp>
/*============================================================================*/
/* FORWARD DECLARATIONS                                                       */
/*============================================================================*/
namespace bembel {

class Texture;
class AssetManager;
class FrameBufferObject;

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

	void SetViewArea(const glm::ivec2& pos, const glm::uvec2& size);
	glm::ivec2 GetViewAreaPosition() const;
	glm::uvec2 GetViewAreaSize() const;

	void Init();
	void Cleanup();

	virtual void Draw(
		const glm::ivec2& viewport_position,
		const glm::uvec2& viewport_size
	) override;

private:
	AssetManager* asset_manager_;
	Texture* texture_;

	glm::ivec2 view_area_pos_;
	glm::uvec2 view_area_size_;

	std::unique_ptr<FrameBufferObject> fbo_;
};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
