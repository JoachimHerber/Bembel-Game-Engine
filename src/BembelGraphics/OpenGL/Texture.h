#ifndef BEMBEL_TEXTURE_H
#define BEMBEL_TEXTURE_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include <BembelOpenGL.h>
#include <BembelConfig.h>

#include <BembelBase/Image.h>
#include <BembelKernel/Assets/AssetManager.h>
#include <BembelKernel/Assets/AssetDescription.h>

#include <glm/glm.hpp>

/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel{

class BEMBEL_API Texture final
{
public:
	Texture(GLenum target = GL_TEXTURE_2D, GLenum format = GL_RGBA8);
	~Texture();

	void Init(
		GLenum minFilter = GL_LINEAR,
		GLenum magFilter = GL_LINEAR,
		GLenum warpS = GL_CLAMP_TO_EDGE,
		GLenum warpT = GL_CLAMP_TO_EDGE);
	void Init(
		const glm::uvec2& size,
		GLenum minFilter = GL_LINEAR,
		GLenum magFilter = GL_LINEAR,
		GLenum warpS = GL_CLAMP_TO_EDGE,
		GLenum warpT = GL_CLAMP_TO_EDGE);
	void Init(
		const Image& data, 
		GLenum minFilter = GL_LINEAR, 
		GLenum magFilter = GL_LINEAR,
		GLenum warpS = GL_CLAMP_TO_EDGE,
		GLenum warpT = GL_CLAMP_TO_EDGE);
	void Cleanup();

	void Bind() const;
	void Release() const;

	bool SetSize(const glm::ivec2& size);
	bool SetData(const Image&, GLint mipMapLevel = 0);

	GLuint GetTextureHandle() const;
	GLenum GetTextureFormat() const;
	GLenum GetTextureTarget() const;

	const static std::string& GetTypeName();
	static Texture* LoadeAsset(const AssetDescription&, AssetManager*);

private:
	GLenum _target;
	GLenum _format;
	GLuint _handle;
};

using TextureContainer    = AssetContainer<Texture>;
using TextureContainerPtr = std::shared_ptr<TextureContainer>;

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
