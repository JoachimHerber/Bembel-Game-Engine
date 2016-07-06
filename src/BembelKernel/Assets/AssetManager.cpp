/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "AssetManager.h"

#include <BembelBase/Logging/Logger.h>

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/

namespace bembel{

AssetManager::AssetManager()
{}

AssetManager::~AssetManager()
{}

bool AssetManager::LoadeAsset(
	const std::string& assteTypeName, 
	const AssetDescription& asset)
{
	auto loader = GetAssetLoader(assteTypeName);
	if (loader)
		return loader->LoadeAsset(asset);
	return false;
}

bool AssetManager::UnloadeAsset(
	const std::string& assteTypeName, 
	const std::string& name, bool force /*= false*/)
{
	auto loader = GetAssetLoader(assteTypeName);
	if (loader)
		return loader->UnloadeAsset(name, force);
	return false;
}

unsigned AssetManager::LoadeAssets(
	const std::string& assteTypeName,
	const std::vector<AssetDescription>& assets)
{
	auto loader = GetAssetLoader(assteTypeName);
	if (loader)
		return loader->LoadeAssets(assets);
	return 0;
}

unsigned AssetManager::UnloadeAssets(
	const std::string& assteTypeName,
	const std::vector<std::string>& assetNames, bool force /*= false*/)
{
	auto loader = GetAssetLoader(assteTypeName);
	if (loader)
		return loader->UnloadeAssets(assetNames, force);
	return 0;
}

std::shared_ptr<AssetContainerBase> AssetManager::GetAssetContainer(
	const std::string& assteTypeName)
{
	auto it = _assetTypeMap.find(assteTypeName);
	if (it != _assetTypeMap.end())
	{
		return _assetTypes[it->second].container;
	}
	return nullptr;
}

std::shared_ptr<AssetLoaderBase> AssetManager::GetAssetLoader(
	const std::string& assteTypeName)
{
	auto it = _assetTypeMap.find(assteTypeName);
	if (it != _assetTypeMap.end())
	{
		return _assetTypes[it->second].loader;
	}
	return nullptr;
}

bool AssetManager::SetAssetLoader(
	const std::string& assteTypeName, 
	std::shared_ptr<AssetLoaderBase> loader)
{
	auto it = _assetTypeMap.find(assteTypeName);
	if (it != _assetTypeMap.end())
	{
		_assetTypes[it->second].loader = loader;
		return true;
	}
	return false;
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
