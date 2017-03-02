#ifndef BEMBEL_ASSETFILELOCATOR_H
#define BEMBEL_ASSETFILELOCATOR_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include <BembelConfig.h>
#include <BembelBase/XML.h>

#include <cassert>
#include <string>
#include <vector>
#include <unordered_map>

/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel{

class BEMBEL_API AssetFileLocator final
{
public:
public:
	AssetFileLocator();
	~AssetFileLocator();
	
	bool Init( xml::Element* properties );

	template <typename AssetType>
	void AddAssetDirectory( const std::string& directory );
	void AddAssetDirectory( const std::string& assetTypeName, const std::string& directory );
	
	void AddGenericAssetDirectory( const std::string& directory );

	template <typename AssetType>
	bool FindAssetLocation( const std::string& fileName, std::string* fullFilePath);
	bool FindAssetLocation( const std::string& assetTypeName, const std::string& fileName, std::string* fullFilePath );

private:
	std::unordered_map<std::string, std::vector<std::string>>  _type_specivic_asset_directories;

	std::vector<std::string> _generic_asset_directories;
};

} //end of namespace bembel
/*============================================================================*/
/* TEMPLATE IMPLEMENTATION                                                    */
/*============================================================================*/
namespace bembel {

template<typename AssetType>
inline void AssetFileLocator::AddAssetDirectory( const std::string& directory )
{
	return AddAssetDirectory( AssetType::GetTypeName(), directory );
}

template<typename AssetType>
inline bool AssetFileLocator::FindAssetLocation( 
	const std::string& fileName, std::string* fullFilePath )
{
	return FindAssetLocation( AssetType::GetTypeName(), fileName, fullFilePath );
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
