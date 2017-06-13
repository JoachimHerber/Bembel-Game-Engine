/******************************************************************************/
/* ************************************************************************** */
/* *                                                                        * */
/* *    MIT License                                                         * */
/* *                                                                        * */
/* *   Copyright(c) 2017 Joachim Herber                                     * */
/* *                                                                        * */
/* *   Permission is hereby granted, free of charge, to any person          * */
/* *   obtaining copy of this software and associated documentation files   * */
/* *   (the "Software"), to deal in the Software without restriction,       * */
/* *   including without limitation the rights to use, copy, modify, merge, * */
/* *   publish, distribute, sublicense, and/or sell copies of the Software, * */
/* *   and to permit persons to whom the Software is furnished to do so,    * */
/* *   subject to the following conditions :                                * */
/* *                                                                        * */
/* *   The above copyright notice and this permission notice shall be       * */
/* *   included in all copies or substantial portions of the Software.      * */
/* *                                                                        * */
/* *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,      * */
/* *   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF   * */
/* *   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND                * */
/* *   NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS   * */
/* *   BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN   * */
/* *   ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN    * */
/* *   CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE     * */
/* *   SOFTWARE.                                                            * */
/* *                                                                        * */
/* ************************************************************************** */
/******************************************************************************/

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
