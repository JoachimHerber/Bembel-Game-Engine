#ifndef BEMBEL_GRAPHICS_GEOMETRY_GEOMETRYRENDERER_H_
#define BEMBEL_GRAPHICS_GEOMETRY_GEOMETRYRENDERER_H_
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "bembel-config.h"
#include "bembel-open-gl.h"

#include <memory>
#include <vector>

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

	virtual std::unique_ptr<Material> CreateMaterial(const xml::Element* propertiey) = 0;

protected:
	AssetManager* asset_manager_;
	const unsigned id_;
};

class BEMBEL_API DefaultGeometryRenderer : public GeometryRendererBase
{
public:
	DefaultGeometryRenderer(AssetManager* asset_manager, unsigned id);
	~DefaultGeometryRenderer();

	bool SetShader(AssetHandle);

	virtual void Render(
		const glm::mat4& proj,
		const glm::mat4& view,
		const std::vector<GeometryRenderData>& data) override;

	virtual std::unique_ptr<Material> CreateMaterial(
		const xml::Element* propertiey) override;

	static std::unique_ptr<DefaultGeometryRenderer>
		CreateRenderer(const xml::Element*, AssetManager*, unsigned id);

private:
	AssetHandle shader_;

	GLuint material_uniform_block_index_;
	GLuint material_uniform_buffer_size_;
};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
