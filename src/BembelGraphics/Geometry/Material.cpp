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

MaterialLoader::MaterialLoader(
	MaterialContainerPtr material,
	TextureContainerPtr  textures)
	: _materials(material)
	, _textures(textures)
{}

MaterialLoader::~MaterialLoader()
{
	//delete all loaded assets
	for (auto it: _loadedMaterials)
	{
		Material* mat = _materials->RemoveAsset(it.second, true);
		if (mat)
			delete mat;
	}
}

void MaterialLoader::CreateDummyMaterial()
{
	if (_materials->HasAsset("dummy"))
		return;

	Material* mat = new Material();

	AssetHandle handle = _materials->AddAsset(mat, "dummy");
	_materials->SetDummyAsset(handle);
	_loadedMaterials.emplace("dummy", handle);
}

bool MaterialLoader::LoadeAsset(const AssetDescription& asset)
{
	if (_materials->HasAsset(asset.GetName()))
		return false; // there already is an asset with the specified name

	std::string value;

	Material* mat = new Material();

	asset.GetProperty("emission", mat->_emission);
	asset.GetProperty("albedo", mat->_albedo);
	asset.GetProperty("reflectivity", mat->_reflectivity);
	asset.GetProperty("roughness", mat->_roughness);

	if (asset.GetProperty("emission_texture", value))
	{
		mat->_emissionTexture = _textures->GetAssetHandle(value);
		_textures->IncrementAssetRefCount(mat->_emissionTexture);
	}
	if (asset.GetProperty("albedo_texture", value))
	{
		mat->_albedoTexture = _textures->GetAssetHandle(value);
		_textures->IncrementAssetRefCount(mat->_albedoTexture);
	}
	if (asset.GetProperty("reflectivity_texture", value))
	{
		mat->_reflectivityTexture = _textures->GetAssetHandle(value);
		_textures->IncrementAssetRefCount(mat->_reflectivityTexture);
	}
	if (asset.GetProperty("normal_texture", value))
	{
		mat->_normalTexture = _textures->GetAssetHandle(value);
		_textures->IncrementAssetRefCount(mat->_normalTexture);
	}

	asset.GetProperty(
		"normal_map_contains_roughness", 
		mat->_normalMapHasRoughness);

	AssetHandle handle = _materials->AddAsset(mat, asset.GetName());
	_loadedMaterials.emplace(asset.GetName(), handle);
	return true;
}

bool MaterialLoader::UnloadeAsset(const std::string& name, bool force /*= false*/)
{
	auto it = _loadedMaterials.find(name);
	if (it != _loadedMaterials.end())
	{
		Material* mat = _materials->RemoveAsset(it->second, force);
		if (mat)
		{
			_textures->DecrementAssetRefCount(mat->_emissionTexture);
			_textures->DecrementAssetRefCount(mat->_albedoTexture);
			_textures->DecrementAssetRefCount(mat->_reflectivityTexture);
			_textures->DecrementAssetRefCount(mat->_normalTexture);
			delete mat;
			_loadedMaterials.erase(it);
			return true;
		}
	}
	return false;
}

std::shared_ptr<MaterialLoader> MaterialLoader::CreateDefaultLoader(
	AssetManager* assetMgr)
{
	auto materials = assetMgr->RequestAssetContainer<Material>();
	auto textures  = assetMgr->RequestAssetContainer<Texture>();
	auto loader = std::make_shared<MaterialLoader>(materials, textures);
	loader->CreateDummyMaterial();
	return loader;
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
