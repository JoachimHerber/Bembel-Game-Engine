#ifndef BEMBEL_GEOMETRYRENDERQUEUE_H
#define BEMBEL_GEOMETRYRENDERQUEUE_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include <BembelConfig.h>
#include <BembelOpenGL.h>

#include <BembelBase/XML.h>
#include <BembelKernel/Assets/AssetHandle.h>

#include "GeometryRenderer.h"

#include <glm/glm.hpp>

#include <memory>
#include <vector>

/*============================================================================*/
/* FORWARD DECLARATIONS                                                       */
/*============================================================================*/
namespace bembel {

class AssetManager;
class GeometryModel;
class Material;

}//end of namespace bembel
 /*============================================================================*/
 /* CLASS DEFINITIONS                                                          */
 /*============================================================================*/
namespace bembel {

class BEMBEL_API GeometryRenderQueue
{
public:
	GeometryRenderQueue();
	~GeometryRenderQueue();

	void SetAssetMannager( AssetManager* assetMgr);

	bool AddGeometryObject(AssetHandle model, const glm::mat4& transform);

	bool AddGeometryObject(GeometryModel* model, const glm::mat4& transform);

	// sorts the renderables to minimize state changes dureing rendering
	void SortRenderData();
	void ClearRendarData();
	const std::vector<GeometryRenderData>& GetRenderData() const;

private:
	AssetManager* _assetMgr;
	std::vector<GeometryRenderData> _renderData;
};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
