/*============================================================================*/
/* TEMPLATE IMPLEMENTATION                                                    */
/*============================================================================*/
namespace bembel{

template<typename AssetType>
inline SerialAssetLoader<AssetType>::SerialAssetLoader(
	AssetManager*  assetMgr, 
	ContainerType* container)
	: _assetMgr(assetMgr)
	, _container(container)
{}

template<typename AssetType>
inline SerialAssetLoader<AssetType>::~SerialAssetLoader()
{}

template<typename AssetType>
inline AssetHandle SerialAssetLoader<AssetType>::RequestAsset(
	const std::string& fileName )
{
	AssetHandle handle = _container->GetAssetHandle( fileName );

	if( !_container->IsHandelValid( handle ) )
	{
		// we have to load the asset
		std::unique_ptr<AssetType> asset =
			AssetType::LoadAsset( _assetMgr, fileName );
		if( !asset )
			return AssetHandle();

		handle = _container->AddAsset( std::move( asset ) );
		_container->IncrementAssetRefCount( handle );
		_container->RegisterAssetAlias( handle, fileName );
	}

	_container->IncrementAssetRefCount( handle );
	return handle;
}

template<typename AssetType>
inline AssetHandle SerialAssetLoader<AssetType>::RequestAsset(
	const xml::Element* properties )
{
	std::string name = "";
	if (!xml::GetAttribute(properties, "name", name))
		return AssetHandle();

	AssetHandle handle = _container->GetAssetHandle( name );
	if( !_container->IsHandelValid( handle ) )
	{
		// we have to load the asset
		std::unique_ptr<AssetType> asset =
			AssetType::LoadAsset( _assetMgr, properties );
		if( !asset )
			return AssetHandle();

		handle = _container->AddAsset( std::move( asset ) );
		_container->IncrementAssetRefCount( handle );
		_container->RegisterAssetAlias( handle, name );
	}

	_container->IncrementAssetRefCount( handle );
	return handle;
}

template<typename AssetType>
inline bool SerialAssetLoader<AssetType>::ReleaseAsset(
	AssetHandle assetHandel )
{
	if( _container->IsHandelValid( assetHandel ) )
	{
		_container->DecrementAssetRefCount( assetHandel );
		if( _container->GetAssetRefCount( assetHandel ) == 0 )
		{
			AssetType::DeleteAsset( 
				_assetMgr, 
				_container->RemoveAsset( assetHandel ));
			return true;
		}
	}
	return false;
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
