#ifndef BEMBEL_GEOMETRYRENDERER_H
#define BEMBEL_GEOMETRYRENDERER_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include <BembelConfig.h>
#include <BembelOpenGL.h>

#include <BembelBase/XML.h>

#include "../Assets/AssetHandle.h"

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

class BEMBEL_API GeometryRenderer
{
public:
	GeometryRenderer();
	~GeometryRenderer();

	void SetAssetMannager(
		std::shared_ptr<AssetManager> assetMgr);
	void SetShader(std::shared_ptr<Shader>);

	struct GeometryInstance
	{
		AssetHandle model;
		glm::mat4 transformation;
	};

	void DrawGeometry(
		const glm::mat4& view,
		const glm::mat4& proj,
		const std::vector<GeometryInstance>& instances 
	);

	static std::shared_ptr<GeometryRenderer> CreateRenderer( const xml::Element*);

private:
	void DoRendering(const glm::mat4& proj);
	void UseMaterial(Material*  material);

private:
	std::shared_ptr<AssetManager> _assetMgr;
	std::shared_ptr<Shader>       _shader;

	struct RenderData
	{
		GeometryMesh* mesh;
		Material*     material;
		unsigned      first;
		unsigned      count;
		glm::mat4     transform;
	};

	// a static vector for storing render data
	// used to avoid unnecessary memory allocations
	// @TODO replace this with the use of a memory pool
	// once costume memory management is implemented
	static std::vector<RenderData> s_renderData;
};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
