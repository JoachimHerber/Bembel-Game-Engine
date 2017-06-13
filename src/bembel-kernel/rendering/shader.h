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

#ifndef BEMBEL_KERNEL_RENDERING_SHADER_H_
#define BEMBEL_KERNEL_RENDERING_SHADER_H_
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "bembel-config.h"
#include "bembel-open-gl.h"

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

#include <bembel-kernel/assets/serial-asset-loader.hpp>
#include <bembel-kernel/assets/asset-handle.h>

/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel {

class BEMBEL_API Shader
{
public:
	using DefaultLoaderType = SerialAssetLoader<Shader>;

public:
	Shader(GLuint handle);
	virtual ~Shader();

	bool Init(const std::string& source);
		
	GLuint GetHandle() const;

	const static std::string& GetTypeName();

	static std::unique_ptr<Shader> LoadAsset(
		AssetManager* asset_manager, const std::string& path);
	static std::unique_ptr<Shader> CreateAsset(
		AssetManager* asset_manager, const xml::Element* properties);

	static void DeleteAsset(AssetManager*, std::unique_ptr<Shader>)
	{}

	static std::unique_ptr<Shader> CreateShader(
		std::unique_ptr<Shader> properties);
	static std::unique_ptr<Shader> CreateShader(
		GLenum type, const std::string& source);

private:
	GLuint handle_;
};

class BEMBEL_API ShaderProgram final
{
public:
	using DefaultLoaderType = SerialAssetLoader<ShaderProgram>;

public:
	ShaderProgram();
	ShaderProgram(const ShaderProgram&) = delete;
	ShaderProgram& operator& (const ShaderProgram&) = delete;
	~ShaderProgram();

	bool AttachShader(AssetManager* asset_manager, AssetHandle handle);

	void BindAttribLocation(const std::string& name, unsigned int index);
	void BindFragDataLocation(const std::string& name, unsigned int index);

	GLint  GetUniformLocation(const std::string& name) const;
	GLuint GetUniformBlockIndex(const std::string& name) const;
	GLint  GetUniformBlockDataSize(GLuint block_index) const;
	GLint  GetUniformBlockActiveUniforms(GLuint block_index) const;
	void   GetUniformBlockActiveUniformIndices(
		GLuint block_index, std::vector<GLint>* indices) const;

	GLint GetActiveUniformOffset(GLuint uniform_index) const;
	void  GetActiveUniform(
		GLuint uniform_index, GLint* size, GLenum* type, std::string* name) const;

	bool Link();

	bool Use();

	const static std::string& GetTypeName();

	static std::unique_ptr<ShaderProgram> LoadAsset(
		AssetManager* asset_manager, const std::string& path);
	static std::unique_ptr<ShaderProgram> CreateAsset(
		AssetManager* asset_manager, const xml::Element* properties);

	static void DeleteAsset(AssetManager*, std::unique_ptr<ShaderProgram>);

private:
	GLuint                   program_handle_;
	std::vector<AssetHandle> shader_handles_;

	bool ready_to_use_ = false;

	mutable std::unordered_map<std::string, GLint> uniorm_locations_;
};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
