/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "geometry-model.h"
#include "geometry-mesh.h"

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel {

GeometryModel::GeometryModel()
{}

GeometryModel::~GeometryModel()
{}

AssetHandle GeometryModel::GetMesh()
{
	return _mesh;
}

const std::vector<GeometryModel::MaterialMapping>& GeometryModel::GetMateialMapping()
{
	return _materialMapping;
}

const std::string& GeometryModel::GetTypeName()
{
	const static std::string typeName = "GeometryModel";
	return typeName;
}

std::unique_ptr<GeometryModel> GeometryModel::CreateAsset(
	AssetManager* assetMgr, const xml::Element* properties)
{
	auto model = std::make_unique<GeometryModel>();

	std::string mesh;
	xml::GetAttribute(properties, "mesh", mesh);
	model->_mesh = assetMgr->RequestAsset<GeometryMesh>(mesh);

	for (const xml::Element* mapping : xml::IterateChildElements(properties, "MaterialMapping"))
	{
		std::string mat, subMesh;
		xml::GetAttribute(mapping, "material", mat);
		xml::GetAttribute(mapping, "submesh", subMesh);
		model->_materialMapping.push_back(MaterialMapping{
			assetMgr->RequestAsset<Material>(mat), subMesh
		});
	}

	return std::move(model);
}

std::unique_ptr<GeometryModel> GeometryModel::LoadFromFile(
	AssetManager* assetMgr, const std::string& fileName)
{
	xml::Document doc;
	if (doc.LoadFile(fileName.c_str()) != tinyxml2::XML_SUCCESS)
	{
		BEMBEL_LOG_ERROR()
			<< "Failed to load file '" << fileName << "'\n"
			<< doc.ErrorName() << std::endl;
		return false;
	}

	const xml::Element* root = doc.FirstChildElement("GeometryModel");
	if (!root)
	{
		BEMBEL_LOG_ERROR()
			<< "File '" << fileName << "' has no root element 'GeometryModel'"
			<< std::endl;
		return false;
	}
	return CreateAsset(assetMgr, root);
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
