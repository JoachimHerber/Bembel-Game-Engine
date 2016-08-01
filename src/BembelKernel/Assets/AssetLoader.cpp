/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "AssetLoader.h"

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel {

AssetLoaderBase::AssetLoaderBase(const std::string& name, AssetManager* assetMgr)
	: _name(name)
	, _assetMgr(assetMgr)
{}

AssetLoaderBase::~AssetLoaderBase()
{}

unsigned AssetLoaderBase::Loade(const std::vector<AssetDescription>& assets)
{
	int count = 0;
	for (const AssetDescription& asset : assets)
		if (Loade(asset))
			++count;

	return count;
}

const std::string& AssetLoaderBase::GetName() const
{
	return _name;
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
