/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "Material.h"

#include <BembelBase/Conversion.h>

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel {

Material::Material()
{}

Material::~Material()
{}

const Material::Color& Material::GetEmission() const
{
	return _emission;
}

void Material::SetEmission(const Color& val)
{
	_emission = val;
}

const Material::Color& Material::GetAlbedo() const
{
	return _albedo;
}

void Material::SetAlbedo(const Color& value)
{
	_albedo = value;
}

const Material::Color& Material::GetReflectivity() const
{
	return _reflectivity;
}

void Material::SetReflectivity(const Color& value)
{
	_reflectivity = value;
}

float Material::GetRoughness() const
{
	return _roughness;
}

void Material::SetRoughness(float value)
{
	_roughness = value;
}

AssetHandle Material::GetEmissionTexture() const
{
	return _emissionTexture;
}

void Material::SetEmissionTexture(AssetHandle handle)
{
	_emissionTexture = handle;
}

AssetHandle Material::GetAlbedoTexture() const
{
	return _albedoTexture;
}

void Material::SetAlbedoTexture(AssetHandle handle)
{
	_albedoTexture = handle;
}

AssetHandle Material::GetReflectivityTexture() const
{
	return _reflectivityTexture;
}

void Material::SetReflectivityTexture(AssetHandle handle)
{
	_reflectivityTexture = handle;
}

AssetHandle Material::GetNormalTexture() const
{
	return _normalTexture;
}

void Material::SetNormalTexture(AssetHandle handle)
{
	_normalTexture = handle;
}

bool Material::GetNormalMapHasRoughness() const
{
	return _normalMapHasRoughness;
}

void Material::SetNormalMapHasRoughness(bool value)
{
	_normalMapHasRoughness = value;
}

const std::string& Material::GetTypeName()
{
	const static std::string typeName = "Material";
	return typeName;
}

std::unique_ptr<Material> Material::CreateAsset(
	AssetManager* assetMgr,
	const xml::Element* properties)
{
	auto mat = std::make_unique<Material>();

	xml::GetAttribute(properties, "emission", mat->_emission);
	xml::GetAttribute(properties, "albedo", mat->_albedo);
	xml::GetAttribute(properties, "reflectivity", mat->_reflectivity);
	xml::GetAttribute(properties, "roughness", mat->_roughness);
	xml::GetAttribute(properties, "normal_map_contains_roughness", mat->_normalMapHasRoughness);

// 	std::string texture;
// 	if (xml::GetAttribute(properties, "emission_texture", texture))
// 	{
// 		mat->_emissionTexture =
// 			assetMgr->RequestAsset<Texture>(texture);
// 		assetMgr->IncrementAssetRefCount(mat->_emissionTexture);
// 	}
// 	if (xml::GetAttribute(properties, "albedo_texture", texture))
// 	{
// 		mat->_albedoTexture =
// 			assetMgr->RequestAsset<Texture>(texture);
// 		assetMgr->IncrementAssetRefCount(mat->_albedoTexture);
// 	}
// 	if (xml::GetAttribute(properties, "reflectivity_texture", texture))
// 	{
// 		mat->_reflectivityTexture =
// 			assetMgr->RequestAsset<Texture>(texture);
// 		assetMgr->IncrementAssetRefCount(mat->_reflectivityTexture);
// 	}
// 	if (xml::GetAttribute(properties, "normal_texture", texture))
// 	{
// 		mat->_normalTexture =
// 			assetMgr->RequestAsset<Texture>(texture);
// 		assetMgr->IncrementAssetRefCount(mat->_normalTexture);
// 	}

	return std::move(mat);
}

std::unique_ptr<Material> Material::LoadFromFile(
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

	const xml::Element* root = doc.FirstChildElement("Material");
	if (!root)
	{
		BEMBEL_LOG_ERROR()
			<< "File '" << fileName << "' has no root element 'Material'"
			<< std::endl;
		return false;
	}
	return CreateAsset(assetMgr, root);
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
