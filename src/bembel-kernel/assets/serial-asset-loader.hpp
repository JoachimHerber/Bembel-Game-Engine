#ifndef BEMBEL_KERNEL_ASSETS_SERIALASSETLOADER_H_
#define BEMBEL_KERNEL_ASSETS_SERIALASSETLOADER_H_
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "bembel-config.h"

#include "asset-loader.h"
#include "asset-manager.h"

/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel {

template<typename AssetType>
class SerialAssetLoader : public AssetLoaderBase
{
public:
	using ContainerType = AssetContainer<AssetType>;

	SerialAssetLoader(AssetManager* asset_manager, ContainerType* container);
	virtual ~SerialAssetLoader();

	virtual AssetHandle RequestAsset(const std::string& file_name) override;
	virtual AssetHandle RequestAsset(const xml::Element* properties) override;
	virtual bool ReleaseAsset(AssetHandle asset_handel) override;

	virtual void Update() override;

	virtual void  ResetProgess() override;
	virtual float GetProgress() override;
	virtual bool  LoadingFinished() override;

protected:
	AssetManager*   asset_manager_;
	ContainerType*  container_;
};

} //end of namespace bembel
/*============================================================================*/
/* TEMPLATE IMPLEMENTATION                                                    */
/*============================================================================*/

#include "serial-asset-loader.inl"

/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
