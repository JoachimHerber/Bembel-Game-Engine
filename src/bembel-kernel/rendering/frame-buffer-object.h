#ifndef BEMBEL_KERNEL_RENDERING_RENDERTOTEXTURE_H_
#define BEMBEL_KERNEL_RENDERING_RENDERTOTEXTURE_H_
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "bembel-config.h"
#include "bembel-open-gl.h"

#include <memory>
#include <vector>

#include <glm/glm.hpp>

/*============================================================================*/
/* FORWARD DECLARATIONS                                                       */
/*============================================================================*/
namespace bembel {

class Texture;

}//end of namespace bembel
/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel {

class BEMBEL_API FrameBufferObject
{
public:
	FrameBufferObject();
	~FrameBufferObject();

	void Init();
	void CleanUp();

	void RemoveAllAttechments();
	void SetDepthAttechment(Texture* texture, GLint level = 0);
	void SetColorAttechment(unsigned index, Texture* texture, GLint level = 0);

	void BeginRenderToTexture();
	void EndRenderToTexture();

	void BlitToBackBuffer(
		const glm::ivec2& source_min,
		const glm::ivec2& source_max,
		const glm::ivec2& target_min,
		const glm::ivec2& target_max
	);

private:
	GLuint handle_;

	struct Attechment
	{
		Texture* texture;
		GLint    level;
	};

	Attechment              depth_attechment_;
	std::vector<Attechment> color_attechments_;
};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
