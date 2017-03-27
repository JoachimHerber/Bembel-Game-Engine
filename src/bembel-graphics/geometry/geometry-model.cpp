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
	return mesh_;
}

const std::vector<GeometryModel::MaterialMapping>&
GeometryModel::GetMateialMapping()
{
	return material_mapping_;
}

const std::string& GeometryModel::GetTypeName()
{
	const static std::string type_name = "GeometryModel";
	return type_name;
}

std::unique_ptr<GeometryModel> GeometryModel::LoadAsset(
	AssetManager* asset_manager, const std::string& file_name)
{
	xml::Document doc;
	if( doc.LoadFile(file_name.c_str()) != tinyxml2::XML_SUCCESS )
	{
		BEMBEL_LOG_ERROR()
			<< "Failed to load file '" << file_name << "'\n"
			<< doc.ErrorName() << std::endl;
		return nullptr;
	}

	const xml::Element* root = doc.FirstChildElement("GeometryModel");
	if( !root )
	{
		BEMBEL_LOG_ERROR()
			<< "File '" << file_name << "' has no root element 'GeometryModel'"
			<< std::endl;
		return nullptr;
	}
	return CreateGeometryModel(asset_manager, root);
}

std::unique_ptr<GeometryModel> GeometryModel::CreateAsset(
	AssetManager* asset_manager, const xml::Element* properties)
{
	return CreateGeometryModel(asset_manager, properties);
}

void GeometryModel::DeleteAsset(
	AssetManager* asset_manager,
	std::unique_ptr<GeometryModel> model)
{
	asset_manager->ReleaseAsset(model->mesh_);
}

std::unique_ptr<GeometryModel> GeometryModel::CreateGeometryModel(
	AssetManager* asset_manager, const xml::Element* properties)
{
	auto model = std::make_unique<GeometryModel>();

	std::string mesh;
	xml::GetAttribute(properties, "mesh", mesh);
	model->mesh_ = asset_manager->RequestAsset<GeometryMesh>(mesh);

	for( const xml::Element* mapping : xml::IterateChildElements(
		properties, "MaterialMapping") )
	{
		std::string mat, subMesh;
		xml::GetAttribute(mapping, "material", mat);
		xml::GetAttribute(mapping, "submesh", subMesh);
		model->material_mapping_.push_back(MaterialMapping{
			asset_manager->RequestAsset<Material>(mat), subMesh
		});
	}

	return std::move(model);
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
