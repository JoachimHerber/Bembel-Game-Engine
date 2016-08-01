/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "GeometryModel.h"

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel {

GeometryModel::GeometryModel()
{}

GeometryModel::~GeometryModel()
{}

std::shared_ptr<VertexArrayObject> GeometryModel::GetVAO() const
{
	return _vao;
}

void GeometryModel::SetVAO(std::shared_ptr<VertexArrayObject> vao)
{
	_vao = vao;
}

void GeometryModel::AddSubMesh(const SubMesh& mesh)
{
	_subMeshes.push_back(mesh);
}

const std::vector<GeometryModel::SubMesh>& 
	GeometryModel::GetSubMeshes() const
{
	return _subMeshes;
}

const std::string& GeometryModel::GetTypeName()
{
	const static std::string typeName = "GeometryModel";
	return typeName;
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
