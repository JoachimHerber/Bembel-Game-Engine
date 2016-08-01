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

Material* Material::LoadeAsset(const AssetDescription& asset, AssetManager* mgr)
{
	auto textures = mgr->GetAssetContainer<Texture>();

	Material* mat = new Material();

	asset.GetProperty("emission", mat->_emission);
	asset.GetProperty("albedo", mat->_albedo);
	asset.GetProperty("reflectivity", mat->_reflectivity);
	asset.GetProperty("roughness", mat->_roughness);

	if (textures)
	{
		std::string value;
		if (asset.GetProperty("emission_texture", value))
		{
			mat->_emissionTexture = textures->GetAssetHandle(value);
			textures->IncrementAssetRefCount(mat->_emissionTexture);
		}
		if (asset.GetProperty("albedo_texture", value))
		{
			mat->_albedoTexture = textures->GetAssetHandle(value);
			textures->IncrementAssetRefCount(mat->_albedoTexture);
		}
		if (asset.GetProperty("reflectivity_texture", value))
		{
			mat->_reflectivityTexture = textures->GetAssetHandle(value);
			textures->IncrementAssetRefCount(mat->_reflectivityTexture);
		}
		if (asset.GetProperty("normal_texture", value))
		{
			mat->_normalTexture = textures->GetAssetHandle(value);
			textures->IncrementAssetRefCount(mat->_normalTexture);
		}
	}

	asset.GetProperty(
		"normal_map_contains_roughness",
		mat->_normalMapHasRoughness);

	return mat;
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
