#ifndef BEMBEL_GRAPHICS_GEOMETRY_GEOMETRYRENDERQUEUE_H_
#define BEMBEL_GRAPHICS_GEOMETRY_GEOMETRYRENDERQUEUE_H_
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "bembel-config.h"
#include "bembel-open-gl.h"

#include "geometry-renderer.h"

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
