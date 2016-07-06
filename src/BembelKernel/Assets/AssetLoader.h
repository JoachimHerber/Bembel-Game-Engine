#ifndef BEMBEL_ASSETLOADER_H
#define BEMBEL_ASSETLOADER_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include <BembelConfig.h>

#include "AssetDescription.h"

#include <string>
#include <vector>

/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel {

class BEMBEL_API AssetLoaderBase
{
public:
	AssetLoaderBase()
	{};
	virtual ~AssetLoaderBase()
	{};
	
	virtual bool LoadeAsset(const AssetDescription& asset) = 0;
	virtual bool UnloadeAsset(const std::string& name, bool force = false) = 0;

	virtual unsigned LoadeAssets(const std::vector<AssetDescription>& assets)
	{
		unsigned n = 0; 
		for (auto& asset : assets) if (LoadeAsset(asset)) ++n;
		return n;
	}
	virtual unsigned UnloadeAssets(const std::vector<std::string>& assetNames, bool force = false)
	{
		unsigned n = 0;
		for (auto& assetName : assetNames) if (UnloadeAsset(assetName, force)) ++n;
		return n;
	}

	virtual void Update()
	{};
};


} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
