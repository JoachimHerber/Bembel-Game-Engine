/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "asset-locator.h"

#include <fstream>

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/

namespace bembel {

AssetFileLocator::AssetFileLocator()
{}

AssetFileLocator::~AssetFileLocator()
{}

bool AssetFileLocator::Init(xml::Element* properties)
{
	for( auto it : xml::IterateChildElements(properties, "AssetDirectory") )
	{
		std::string dir, asset_type;
		xml::GetAttribute(it, "directory", dir);
		if( xml::GetAttribute(it, "asset_type", asset_type) )
			AddAssetDirectory(asset_type, dir);
		else
			AddGenericAssetDirectory(dir);
	}
	return true;
}

void AssetFileLocator::AddAssetDirectory(
	const std::string& assetTypeName, const std::string& directory)
{
	type_specivic_asset_directories_[assetTypeName].push_back(directory);
}

void AssetFileLocator::AddGenericAssetDirectory(
	const std::string& directory)
{
	generic_asset_directories_.push_back(directory);
}

namespace {
bool LocateFile(
	const std::vector<std::string>& directories,
	const std::string& fileName,
	std::string* fullFilePath)
{
	for( const std::string& dir : directories )
	{
		std::string path = dir + "/" + fileName;
		std::ifstream infile(path.c_str());
		if( infile.good() )
		{
			if( fullFilePath  != nullptr )
				*fullFilePath = path;

			return true;
		}
	}
	return false;
}
}

bool AssetFileLocator::FindAssetLocation(
	const std::string& assetTypeName,
	const std::string& fileName,
	std::string * fullFilePath)
{
	auto it = type_specivic_asset_directories_.find(assetTypeName);
	if( it != type_specivic_asset_directories_.end() )
	{
		if( LocateFile(it->second, fileName, fullFilePath) )
			return true;
	}
	return LocateFile(generic_asset_directories_, fileName, fullFilePath);
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
