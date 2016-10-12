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

	unsigned LoadAssets(const std::string& fileName);
	
	template<typename AssetType>
	std::shared_ptr<AssetContainer<AssetType>> GetAssetContainer();

	template<typename AssetType>
	AssetHandle RequestAsset(const std::string& filename);

	template<typename AssetType>
	AssetHandle GetAssetHandle(const std::string& name);

	template<typename AssetType>
	AssetType* GetAsset(AssetHandle handle, bool returnDummyIfHandleInvalid = true);

	template<typename AssetType, typename AssetLoaderType = AssetType::DefaultLoaderType>
	bool RegisterAssetType();	

	int GetAssetRefCount(AssetHandle handle);
	void IncrementAssetRefCount(AssetHandle handle);
	void DecrementAssetRefCount(AssetHandle handle);

private:
	std::unordered_map<std::string, uint16_t>  _assetTypeMap;

	std::vector<std::shared_ptr<AssetContainerBase>> _assetContainer;
	std::vector<std::shared_ptr<AssetLoaderBase>>    _assetLoader;
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
