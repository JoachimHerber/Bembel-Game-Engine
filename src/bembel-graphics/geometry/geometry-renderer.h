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

#ifndef BEMBEL_GRAPHICS_GEOMETRY_GEOMETRYRENDERER_H_
#define BEMBEL_GRAPHICS_GEOMETRY_GEOMETRYRENDERER_H_
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "bembel-config.h"
#include "bembel-open-gl.h"

#include <memory>
#include <vector>
#include <map>

#include <glm/glm.hpp>

#include <bembel-base/xml.h>
#include <bembel-kernel/assets/asset-handle.h>

/*============================================================================*/
/* FORWARD DECLARATIONS                                                       */
/*============================================================================*/
namespace bembel {

class AssetManager;
class ShaderProgram;
class GeometryMesh;
class Material;

}//end of namespace bembel
/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel {

struct GeometryRenderData
{
	GeometryMesh* mesh;
	Material*     material;
	unsigned      first;
	unsigned      count;
	glm::mat4     transform;
};

class BEMBEL_API GeometryRendererBase
{
public:
	GeometryRendererBase(AssetManager* asset_manager, unsigned id)
		: asset_manager_{asset_manager}
		, id_{id}
	{};
	virtual ~GeometryRendererBase()
	{};

	unsigned GetRendererID() const
	{
		return id_;
	};

	virtual void Render(
		const glm::mat4& proj,
		const glm::mat4& view,
		const std::vector<GeometryRenderData>& data) = 0;

	virtual std::unique_ptr<Material> CreateMaterial(
		AssetManager* asset_manager, const xml::Element* propertiey) = 0;

protected:
	AssetManager* asset_manager_;
	const unsigned id_;
};

class BEMBEL_API DefaultGeometryRenderer : public GeometryRendererBase
{
public:
	DefaultGeometryRenderer(AssetManager* asset_manager, unsigned id);
	~DefaultGeometryRenderer();

	void AddRequiredTexture(
		const std::string& texture_name, 
		const std::string& uniform_sampler_name);

	bool SetShader(AssetHandle program_handle);

	virtual void Render(
		const glm::mat4& proj,
		const glm::mat4& view,
		const std::vector<GeometryRenderData>& data) override;

	virtual std::unique_ptr<Material> CreateMaterial(
		AssetManager* asset_manager,
		const xml::Element* propertiey) override;

	static std::unique_ptr<DefaultGeometryRenderer>
		CreateRenderer(const xml::Element*, AssetManager*, unsigned id);

private:
	struct MaterialParam
	{
		GLint offset;
		GLint size;
		GLenum type;
	};

	bool ReadMaterialParameter(
		const xml::Element* properties,
		const std::string& param_name,
		const MaterialParam& param, 
		GLbyte* buffer);

private:
	AssetHandle shader_;

	GLuint material_uniform_block_index_;
	GLuint material_uniform_buffer_size_;

	std::map<std::string, MaterialParam> material_params_;

	struct RequiredTexture
	{
		std::string texture_name;
		std::string uniform_sampler_name;
	};
	std::vector<RequiredTexture> required_textures_;
};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
