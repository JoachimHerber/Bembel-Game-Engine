#ifndef BEMBEL_KERNEL_ASSETS_ASSETLOADER_H_
#define BEMBEL_KERNEL_ASSETS_ASSETLOADER_H_
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "bembel-config.h"
#include "asset-handle.h"

#include <memory>
#include <string>
#include <vector>

#include <bembel-base/xml.h>

/*============================================================================*/
/* FORWARD DECLARATIONS                                                       */
/*============================================================================*/
namespace bembel {

class AssetManager;
class AssetContainerBase;

} //end of namespace bembel
/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel {

class BEMBEL_API AssetLoaderBase
{
public:
	AssetLoaderBase();
	virtual ~AssetLoaderBase();

	virtual AssetHandle RequestAsset(const std::string& file_name) = 0;
	virtual AssetHandle RequestAsset(const xml::Element* properties) = 0;
	virtual bool ReleaseAsset(AssetHandle assetHandel) = 0;

	virtual void Update() = 0;

	virtual void  ResetProgess() = 0;
	virtual float GetProgress() = 0;
	virtual bool  LoadingFinished() = 0;

};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
