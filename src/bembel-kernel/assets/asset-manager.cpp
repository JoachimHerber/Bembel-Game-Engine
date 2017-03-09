/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "asset-manager.h"

#include <bembel-base/logging/logger.h>

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/

namespace bembel {

AssetManager::AssetManager()
{}

AssetManager::~AssetManager()
{}

AssetHandle AssetManager::RequestAsset( const std::string & asset_type_name, const std::string & filename )
{
	auto it = _assetTypeMap.find( asset_type_name );
	if( it == _assetTypeMap.end() )
		return AssetHandle();

	return _assetLoader[it->second]->RequestAsset( filename );
}

AssetHandle AssetManager::RequestAsset(
	const std::string& asset_type_name,
	const xml::Element* properties )
{
	auto it = _assetTypeMap.find( asset_type_name );
	if( it == _assetTypeMap.end() )
		return AssetHandle();

	return _assetLoader[it->second]->RequestAsset( properties );
}

bool AssetManager::ReleaseAsset( AssetHandle assetHandel )
{
	uint16_t typeID = assetHandel.typeId;
	if( typeID >= _assetLoader.size() )
		return false;

	return _assetLoader[typeID]->ReleaseAsset( assetHandel );
}

bool AssetManager::IsHandelValid( AssetHandle assetHandel )
{
	uint16_t typeID = assetHandel.typeId;
	if( typeID >= _assetContainer.size() )
		return false;

	return _assetContainer[typeID]->IsHandelValid( assetHandel );
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
