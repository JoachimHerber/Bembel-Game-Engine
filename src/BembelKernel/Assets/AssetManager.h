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

	template<typename AssetType>
	bool LoadeAsset(const AssetDescription& asset);
	bool LoadeAsset(const std::string& assteTypeName, const AssetDescription& asset);

	template<typename AssetType>
	bool UnloadeAsset(const std::string& name, bool force = false);
	bool UnloadeAsset(const std::string& assteTypeName, const std::string& name, bool force = false);

	template<typename AssetType>
	unsigned LoadeAssets(const std::vector<AssetDescription>& assets);
	unsigned LoadeAssets(const std::string& assteTypeName, const std::vector<AssetDescription>& assets);
	unsigned LoadeAssets(const xml::Element*);

	template<typename AssetType>
	unsigned UnloadeAssets(const std::vector<std::string>& assetNames, bool force = false);
	unsigned UnloadeAssets(const std::string& assteTypeName, const std::vector<std::string>& assetNames, bool force = false);

	template<typename AssetType>
	std::shared_ptr<AssetContainer<AssetType>> RequestAssetContainer();
	
	template<typename AssetType>
	std::shared_ptr<AssetContainer<AssetType>> GetAssetContainer();
	std::shared_ptr<AssetContainerBase> GetAssetContainer(const std::string& assteTypeName);

	template<typename AssetType>
	std::shared_ptr<AssetLoaderBase> GetAssetLoader();
	std::shared_ptr<AssetLoaderBase> GetAssetLoader(const std::string& assteTypeName);

	template<typename AssetType>
	bool SetAssetLoader(std::shared_ptr<AssetLoaderBase>);
	bool SetAssetLoader(const std::string& assteTypeName, std::shared_ptr<AssetLoaderBase>);

	template<typename AssetType, typename AssetLoaderType, typename ... TArgs>
	bool InitAssetLoader(TArgs ...);

	template<typename AssetType>
	AssetHandle GetAssetHandle(const std::string& name);

	template<typename AssetType>
	AssetType* GetAsset(AssetHandle handle);

private:
	std::unordered_map<std::string, std::size_t>     _assetTypeMap;
	std::vector<std::shared_ptr<AssetLoaderBase>>    _loader;
	std::vector<std::shared_ptr<AssetContainerBase>> _container;
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
