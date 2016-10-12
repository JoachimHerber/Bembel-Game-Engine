#ifndef BEMBEL_SERIALASSETLOADER_H
#define BEMBEL_SERIALASSETLOADER_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include <BembelConfig.h>

#include "AssetLoader.h"
#include "AssetManager.h"

/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel {

template<typename AssetType>
class SerialAssetLoader : public AssetLoaderBase
{
public:
	using ContainerType    = AssetContainer<AssetType>;
	using ContainerTypePtr = std::shared_ptr<ContainerType>;

	SerialAssetLoader(AssetManager* assetMgr, ContainerTypePtr container);
	virtual ~SerialAssetLoader();

	virtual bool CreateAsset( const xml::Element* ) override;
	virtual AssetHandle RequestAsset( const std::string& fileName ) override;

	virtual void Update() override;

	virtual void  ResetProgess() override;
	virtual float GetProgress() override;
	virtual bool  LoadingFinished() override;

protected:
	AssetManager*    _assetMgr;
	ContainerTypePtr _container;
};

template<typename AssetType>
bool CreateSerialAssetLoader(AssetManager* assetMgr);

} //end of namespace bembel
/*============================================================================*/
/* TEMPLATE IMPLEMENTATION                                                    */
/*============================================================================*/

#include "SerialAssetLoader.inl"

/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
