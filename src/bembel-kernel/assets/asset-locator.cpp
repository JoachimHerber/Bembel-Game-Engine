/******************************************************************************/
/* ************************************************************************** */
/* *                                                                        * */
/* *    MIT License                                                         * */
/* *                                                                        * */
/* *   Copyright(c) 2018 Joachim Herber                                     * */
/* *                                                                        * */
/* *   Permission is hereby granted, free of charge, to any person          * */
/* *   obtaining copy of this software and associated documentation files   * */
/* *   (the "Software"), to deal in the Software without restriction,       * */
/* *   including without limitation the rights to use, copy, modify, merge, * */
/* *   publish, distribute, sublicense, and/or sell copies of the Software, * */
/* *   and to permit persons to whom the Software is furnished to do so,    * */
/* *   subject to the following conditions :                                * */
/* *                                                                        * */
/* *   The above copyright notice and this permission notice shall be       * */
/* *   included in all copies or substantial portions of the Software.      * */
/* *                                                                        * */
/* *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,      * */
/* *   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF   * */
/* *   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND                * */
/* *   NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS   * */
/* *   BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN   * */
/* *   ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN    * */
/* *   CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE     * */
/* *   SOFTWARE.                                                            * */
/* *                                                                        * */
/* ************************************************************************** */
/******************************************************************************/

/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "asset-locator.h"

#include <fstream>

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/

namespace bembel {

AssetLocator::AssetLocator()
{}

AssetLocator::~AssetLocator()
{}

bool AssetLocator::Init(const xml::Element* properties)
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

void AssetLocator::AddAssetDirectory(
	const std::string& assetTypeName, const std::string& directory)
{
	type_specivic_asset_directories_[assetTypeName].push_back(directory);
}

void AssetLocator::AddGenericAssetDirectory(
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

bool AssetLocator::FindAssetLocation(
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
