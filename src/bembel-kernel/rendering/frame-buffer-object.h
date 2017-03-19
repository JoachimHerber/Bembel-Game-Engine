#ifndef BEMBEL_KERNEL_RENDERING_RENDERTOTEXTURE_H_
#define BEMBEL_KERNEL_RENDERING_RENDERTOTEXTURE_H_
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "bembel-config.h"
#include "bembel-open-gl.h"

#include <memory>
#include <vector>

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
	using TexturePtr = std::shared_ptr<Texture>;

	FrameBufferObject();
	~FrameBufferObject();

	void Init();
	void CleanUp();

	void RemoveAllAttechments();
	void SetDepthAttechment(TexturePtr texture, GLint level = 0);
	void SetColorAttechment(unsigned index, TexturePtr texture, GLint level = 0);

	void BeginRenderToTexture();
	void EndRenderToTexture();

private:
	GLuint handle_;

	struct Attechment
	{
		TexturePtr texture;
		GLint      level;
	};

	Attechment              depth_attechment_;
	std::vector<Attechment> color_attechments_;
};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
