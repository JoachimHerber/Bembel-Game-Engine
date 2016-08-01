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
/* FORWARD DECLARATIONS                                                       */
/*============================================================================*/
namespace bembel{

class AssetManager;

} //end of namespace bembel
/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel {

class BEMBEL_API AssetLoaderBase
{
public:
	AssetLoaderBase(const std::string& name, AssetManager* );
	virtual ~AssetLoaderBase();
	
	virtual bool     Loade(const AssetDescription& asset) = 0;
	virtual unsigned Loade(const std::vector<AssetDescription>& assets);

	virtual void Update() = 0;

	virtual void  ResetProgess() = 0;
	virtual float GetProgress() = 0;
	virtual bool  LoadingFinished() = 0;

	const std::string& GetName() const;

protected:
	const std::string _name;
	AssetManager* _assetMgr;
};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
