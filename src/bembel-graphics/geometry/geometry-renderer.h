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
