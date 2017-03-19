#ifndef BEMBEL_KERNEL_RENDERING_TEXTURE_H_
#define BEMBEL_KERNEL_RENDERING_TEXTURE_H_
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "bembel-config.h"
#include "bembel-open-gl.h"

#include <bembel-base/image.h>
#include <bembel-kernel/assets/asset-manager.h>

#include <glm/glm.hpp>

/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel {

class BEMBEL_API Texture final
{
public:
	Texture(GLenum target = GL_TEXTURE_2D, GLenum format = GL_RGBA8);
	~Texture();

	void Init(
		GLenum min_filter = GL_LINEAR,
		GLenum mag_filter = GL_LINEAR,
		GLenum warp_s = GL_CLAMP_TO_EDGE,
		GLenum warp_t = GL_CLAMP_TO_EDGE);
	void Init(
		const glm::uvec2& size,
		GLenum min_filter = GL_LINEAR,
		GLenum mag_filter = GL_LINEAR,
		GLenum warp_s = GL_CLAMP_TO_EDGE,
		GLenum warp_t = GL_CLAMP_TO_EDGE);
	void Init(
		const Image& data,
		GLenum min_filter = GL_LINEAR,
		GLenum mag_filter = GL_LINEAR,
		GLenum warp_s = GL_CLAMP_TO_EDGE,
		GLenum warp_t = GL_CLAMP_TO_EDGE);
	void Cleanup();

	void Bind() const;
	void Release() const;

	bool SetSize(const glm::ivec2& size);
	bool SetData(const Image&, GLint mip_map_level = 0);

	GLuint GetTextureHandle() const;
	GLenum GetTextureFormat() const;
	GLenum GetTextureTarget() const;

	const static std::string& GetTypeName();
	//static Texture* LoadeAsset(const AssetDescription&, AssetManager*);

private:
	GLenum target_;
	GLenum format_;
	GLuint handle_;
};

using TextureContainer = AssetContainer<Texture>;
using TextureContainerPtr = std::shared_ptr<TextureContainer>;

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
