/*============================================================================*/
/* TEMPLATE IMPLEMENTATION                                                    */
/*============================================================================*/
namespace bembel{

template<typename AssetType>
inline SerialAssetLoader<AssetType>::SerialAssetLoader(
	AssetManager* assetMgr)
	: AssetLoaderBase(AssetType::GetTypeName(), assetMgr)
	, _container(assetMgr->RequestAssetContainer<AssetType>())
{}

template<typename AssetType>
inline SerialAssetLoader<AssetType>::~SerialAssetLoader()
{}

template<typename AssetType>
inline bool SerialAssetLoader<AssetType>::Loade(
	const AssetDescription& asset)
{
	if (_container->HasAsset(asset.GetName()))
		return false; // there already is an asset with the specified name

	AssetType* assetData = AssetType::LoadeAsset(asset, _assetMgr);
	if (assetData!=nullptr)
	{
		_container->AddAsset(assetData, asset.GetName());
		return true;
	}
	else
	{
		return false;
	}
}

template<typename AssetType>
inline void SerialAssetLoader<AssetType>::Update()
{}

template<typename AssetType>
inline void SerialAssetLoader<AssetType>::ResetProgess()
{}

template<typename AssetType>
inline float SerialAssetLoader<AssetType>::GetProgress()
{
	return 1.0f;
}
template<typename AssetType>
inline bool SerialAssetLoader<AssetType>::LoadingFinished()
{
	return true;
}

template<typename AssetType>
inline bool CreateSerialAssetLoader(AssetManager* assetMgr)
{
	auto loader = std::make_shared<SerialAssetLoader<AssetType>>(assetMgr);
	return assetMgr->AddAssetLoader(loader);
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
