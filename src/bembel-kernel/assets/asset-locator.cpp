/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "asset-locator.h"

#include <fstream>

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/

namespace bembel{

AssetFileLocator::AssetFileLocator()
{}

AssetFileLocator::~AssetFileLocator()
{}

bool AssetFileLocator::Init( xml::Element* properties )
{
	for( auto it : xml::IterateChildElements( properties, "AssetDirectory") )
	{
		std::string dir, asset_type;
		xml::GetAttribute( it, "directory", dir );
		if(xml::GetAttribute( it, "asset_type", asset_type ))
			AddAssetDirectory(asset_type, dir);
		else
			AddGenericAssetDirectory( dir );
	}
	return true;
}

void AssetFileLocator::AddAssetDirectory( 
	const std::string& assetTypeName, const std::string& directory )
{
	_type_specivic_asset_directories[assetTypeName].push_back( directory );
}

void AssetFileLocator::AddGenericAssetDirectory(
	const std::string& directory )
{
	_generic_asset_directories.push_back( directory );
}

namespace {
bool LocateFile(
	const std::vector<std::string>& directories,
	const std::string& fileName,
	std::string* fullFilePath )
{
	for( const std::string& dir : directories )
	{
		std::string path = dir + "/" + fileName;
		std::ifstream infile( path.c_str() );
		if( infile.good() )
		{
			if(fullFilePath  != nullptr)
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
	std::string * fullFilePath )
{
	auto it = _type_specivic_asset_directories.find( assetTypeName );
	if( it != _type_specivic_asset_directories.end() )
	{
		if( LocateFile( it->second, fileName, fullFilePath ) )
			return true;
	}
	return LocateFile( _generic_asset_directories, fileName, fullFilePath );
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
