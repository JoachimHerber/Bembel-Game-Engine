#ifndef BEMBEL_KERNEL_ASSETS_ASSETFILELOCATOR_H_
#define BEMBEL_KERNEL_ASSETS_ASSETFILELOCATOR_H_
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

class BEMBEL_API AssetFileLocator final
{
public:
public:
	AssetFileLocator();
	~AssetFileLocator();

	bool Init(xml::Element* properties);

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
inline void AssetFileLocator::AddAssetDirectory(const std::string& directory)
{
	return AddAssetDirectory(AssetType::GetTypeName(), directory);
}

template<typename AssetType>
inline bool AssetFileLocator::FindAssetLocation(
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
