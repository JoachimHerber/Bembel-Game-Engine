/******************************************************************************/
/* ************************************************************************** */
/* *                                                                        * */
/* *    MIT License                                                         * */
/* *                                                                        * */
/* *   Copyright(c) 2017 Joachim Herber                                     * */
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

#ifndef BEMBEL_KERNEL_ASSETS_ASSETLOCATOR_H_
#define BEMBEL_KERNEL_ASSETS_ASSETLOCATOR_H_
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "bembel-config.h"

#include <cassert>
#include <string>
#include <vector>
#include <unordered_map>

#include <bembel-base/xml.h>

/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel {

class BEMBEL_API AssetLocator final
{
public:
public:
	AssetLocator();
	~AssetLocator();

	bool Init(const xml::Element* properties);

	template <typename AssetType>
	void AddAssetDirectory(const std::string& directory);
	void AddAssetDirectory(const std::string& asset_type_name,
		const std::string& directory);

	void AddGenericAssetDirectory(const std::string& directory);

	template <typename AssetType>
	bool FindAssetLocation(
		const std::string& file_name, std::string* full_file_path);
	bool FindAssetLocation(
		const std::string& asset_type_name,
		const std::string& file_name,
		std::string* full_file_path);

private:
	std::unordered_map<std::string, std::vector<std::string>>
		type_specivic_asset_directories_;

	std::vector<std::string> generic_asset_directories_;
};

} //end of namespace bembel
/*============================================================================*/
/* TEMPLATE IMPLEMENTATION                                                    */
/*============================================================================*/
namespace bembel {

template<typename AssetType>
inline void AssetLocator::AddAssetDirectory(const std::string& directory)
{
	return AddAssetDirectory(AssetType::GetTypeName(), directory);
}

template<typename AssetType>
inline bool AssetLocator::FindAssetLocation(
	const std::string& file_name, std::string* full_file_path)
{
	return FindAssetLocation(
		AssetType::GetTypeName(), file_name, full_file_path);
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
