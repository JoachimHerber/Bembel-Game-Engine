#ifndef BEMBEL_KERNEL_ASSETS_ASSETMANAGER_H_
#define BEMBEL_KERNEL_ASSETS_ASSETMANAGER_H_
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "bembel-config.h"

#include "asset-container.h"
#include "asset-loader.h"
#include "asset-locator.h"

#include <cassert>
#include <string>
#include <vector>
#include <unordered_map>

#include <bembel-base/xml.h>
#include <bembel-base/logging/logger.h>

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
	AssetHandle RequestAsset(const std::string& filename);
	template<typename AssetType>
	AssetHandle RequestAsset( const xml::Element* properties );

	AssetHandle RequestAsset( const std::string& asset_type_name, const std::string& filename );
	AssetHandle RequestAsset( const std::string& asset_type_name, const xml::Element* properties );

	bool ReleaseAsset( AssetHandle assetHandel);

	bool IsHandelValid( AssetHandle assetHandel );

	template<typename AssetType>
	AssetHandle GetAssetHandle(const std::string& name);

	template<typename AssetType>
	AssetType* GetAsset(AssetHandle handle, bool returnDummyIfHandleInvalid = true);

	template<typename AssetType, typename ... TArgs>
	bool RegisterAssetType( TArgs ... args );
	template<typename AssetType, typename AssetLoaderType,  typename ... TArgs>
	bool RegisterAssetType(TArgs ... args);

private:
	template<typename AssetType>
	AssetContainer<AssetType>* GetAssetContainer();

private:
	std::unordered_map<std::string, uint16_t>  _assetTypeMap;

	std::vector<std::shared_ptr<AssetContainerBase>> _assetContainer;
	std::vector<std::shared_ptr<AssetLoaderBase>>    _assetLoader;
};

} //end of namespace bembel
/*============================================================================*/
/* TEMPLATE IMPLEMENTATION                                                    */
/*============================================================================*/

#include "asset-manager.inl"

/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
