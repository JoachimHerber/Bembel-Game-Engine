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

AssetHandle AssetManager::RequestAsset(const std::string & asset_type_name, const std::string & filename)
{
	auto it = asset_type_map_.find(asset_type_name);
	if( it == asset_type_map_.end() )
		return AssetHandle();

	return asset_loader_[it->second]->RequestAsset(filename);
}

AssetHandle AssetManager::RequestAsset(
	const std::string& asset_type_name,
	const xml::Element* properties)
{
	auto it = asset_type_map_.find(asset_type_name);
	if( it == asset_type_map_.end() )
		return AssetHandle();

	return asset_loader_[it->second]->RequestAsset(properties);
}

bool AssetManager::ReleaseAsset(AssetHandle asset_handel)
{
	if( asset_handel.type_id >= asset_loader_.size() )
		return false;

	return asset_loader_[asset_handel.type_id]->ReleaseAsset(asset_handel);
}

bool AssetManager::IsHandelValid(AssetHandle asset_handel)
{
	if( asset_handel.type_id >= asset_container_.size() )
		return false;

	return asset_container_[asset_handel.type_id]->IsHandelValid(asset_handel);
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
