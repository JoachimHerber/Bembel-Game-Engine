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
	const std::string& loaderName,
	const AssetDescription& asset)
{
	auto it = _loader.find(loaderName);
	if (it != _loader.end())
		return it->second->Loade(asset);
	else
		return false;
}

unsigned AssetManager::LoadeAssets(
	const std::string& loaderName,
	const std::vector<AssetDescription>& assets)
{
	auto it = _loader.find(loaderName);
	if (it != _loader.end())
		return it->second->Loade(assets);
	else
		return false;
}

unsigned AssetManager::LoadeAssets(const xml::Element* root)
{
	if (root == nullptr)
		return 0;

	unsigned sum = 0;
	for (auto it : xml::IterateChildElements(root))
	{
		if (LoadeAsset(it->Value(), AssetDescription::Parse(it)))
			++sum;
	}
	
	return sum;
}

std::shared_ptr<AssetContainerBase> AssetManager::GetAssetContainer(
	const std::string& assteTypeName)
{
	auto it = _assetTypeMap.find(assteTypeName);
	if (it != _assetTypeMap.end())
	{
		return _container[it->second];
	}
	return nullptr;
}

bool AssetManager::AddAssetLoader(
	std::shared_ptr<AssetLoaderBase> loader,
	bool overrideExisting)
{
	auto it = _loader.find(loader->GetName());
	if (it == _loader.end())
	{
		_loader.emplace(loader->GetName(), loader);
		return true;
	}
	if (overrideExisting)
	{
		it->second = loader;
		return true;
	}
	return false;
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
