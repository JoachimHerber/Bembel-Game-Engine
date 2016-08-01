#ifndef BEMBEL_ASSETMANAGER_H
#define BEMBEL_ASSETMANAGER_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include <BembelConfig.h>
#include <BembelBase/XML.h>
#include <BembelBase/Logging/Logger.h>

#include <cassert>
#include <string>
#include <vector>
#include <unordered_map>

#include "AssetContainer.h"
#include "AssetLoader.h"

/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel{

class BEMBEL_API AssetManager final
{
public:
public:
	AssetManager();
	~AssetManager();

	bool LoadeAsset(
		const std::string& loaderName,
		const AssetDescription& asset);
	unsigned LoadeAssets(
		const std::string& loaderName, 
		const std::vector<AssetDescription>& assets);
	unsigned LoadeAssets(const xml::Element*);

	template<typename AssetType>
	std::shared_ptr<AssetContainer<AssetType>> RequestAssetContainer();
	
	template<typename AssetType>
	std::shared_ptr<AssetContainer<AssetType>> GetAssetContainer();
	std::shared_ptr<AssetContainerBase> GetAssetContainer(
		const std::string& assteTypeName);

	template<typename AssetType>
	AssetHandle GetAssetHandle(const std::string& name);

	template<typename AssetType>
	AssetType* GetAsset(AssetHandle handle);

	bool AddAssetLoader(
		std::shared_ptr<AssetLoaderBase>, 
		bool overrideExisting = true);

private:
	std::unordered_map<std::string, std::size_t>     _assetTypeMap;
	std::vector<std::shared_ptr<AssetContainerBase>> _container;

	std::unordered_map<std::string, std::shared_ptr<AssetLoaderBase>> _loader;
};

} //end of namespace bembel
/*============================================================================*/
/* TEMPLATE IMPLEMENTATION                                                    */
/*============================================================================*/

#include "AssetManager.inl"

/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
