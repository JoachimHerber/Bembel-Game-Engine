#ifndef BEMBEL_GEOMETRYMODEL_H
#define BEMBEL_GEOMETRYMODEL_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include <BembelOpenGL.h>
#include <BembelConfig.h>

#include <BembelKernel/Assets/AssetHandle.h>
#include <BembelKernel/Assets/AssetContainer.h>

#include "Material.h"

#include <memory>
#include <vector>

/*============================================================================*/
/* FORWARD DECLARATIONS                                                       */
/*============================================================================*/
namespace bembel{

class VertexArrayObject;

}//end of namespace bembel
/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel{

class BEMBEL_API GeometryModel
{
public:
	GeometryModel();
	~GeometryModel();

	std::shared_ptr<VertexArrayObject> GetVAO() const;
	void SetVAO(std::shared_ptr<VertexArrayObject> vao);

	struct SubMesh
	{
		unsigned firstIndex;
		unsigned numIndices;
		GLenum   primitiveType;
		AssetHandle material;
	};

	void AddSubMesh(const SubMesh&);
	const std::vector<SubMesh>& GetSubMeshes() const;

	const static std::string& GetTypeName();
private:
	std::shared_ptr<VertexArrayObject> _vao;
	std::vector<SubMesh>               _subMeshes;
};

using GeometryModelContainer    = AssetContainer<GeometryModel>;
using GeometryModelContainerPtr = std::shared_ptr<GeometryModelContainer>;


} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
