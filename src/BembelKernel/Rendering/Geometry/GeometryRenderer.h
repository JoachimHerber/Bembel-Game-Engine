#ifndef BEMBEL_GEOMETRYRENDERER_H
#define BEMBEL_GEOMETRYRENDERER_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include <BembelConfig.h>
#include <BembelOpenGL.h>

#include <BembelBase/XML.h>

#include "../../Assets/AssetHandle.h"

#include <glm/glm.hpp>

#include <memory>
#include <vector>

/*============================================================================*/
/* FORWARD DECLARATIONS                                                       */
/*============================================================================*/
namespace bembel{

class AssetManager;
class Shader;
class GeometryMesh;
class Material;

}//end of namespace bembel
/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel{

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
	GeometryRendererBase(unsigned id) : _id(id){};
	virtual ~GeometryRendererBase() {};

	unsigned GetRendererID() const { return _id; };

	virtual void Render(
		const glm::mat4& proj,
		const glm::mat4& view,
		const std::vector<GeometryRenderData>& data) = 0;

protected:
	const unsigned _id;
};

class BEMBEL_API DefaultGeometryRenderer : public GeometryRendererBase
{
public:
	DefaultGeometryRenderer(unsigned id);
	~DefaultGeometryRenderer();

	void SetShader(std::shared_ptr<Shader>);


	virtual void Render(
		const glm::mat4& proj,
		const glm::mat4& view,
		const std::vector<GeometryRenderData>& data) override;

	static std::unique_ptr<DefaultGeometryRenderer> 
		CreateRenderer( const xml::Element*, unsigned id);

private:
	void UseMaterial(Material*  material);

private:
	std::shared_ptr<Shader> _shader;
};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
