/*============================================================================*/
/* TEMPLATE IMPLEMENTATION                                                    */
/*============================================================================*/
namespace bembel {

template<typename AssetType>
inline SerialAssetLoader<AssetType>::SerialAssetLoader(
	AssetManager*  asset_manager,
	ContainerType* container)
	: asset_manager_(asset_manager)
	, container_(container)
{}

template<typename AssetType>
inline SerialAssetLoader<AssetType>::~SerialAssetLoader()
{}

template<typename AssetType>
inline AssetHandle SerialAssetLoader<AssetType>::RequestAsset(
	const std::string& file_name)
{
	AssetHandle handle = container_->GetAssetHandle(file_name);

	if( !container_->IsHandelValid(handle) )
	{
		// we have to load the asset
		std::unique_ptr<AssetType> asset =
			AssetType::LoadAsset(asset_manager_, file_name);
		if( !asset )
			return AssetHandle();

		handle = container_->AddAsset(std::move(asset));
		container_->IncrementAssetRefCount(handle);
		container_->RegisterAssetAlias(handle, file_name);
	}

	container_->IncrementAssetRefCount(handle);
	return handle;
}

template<typename AssetType>
inline AssetHandle SerialAssetLoader<AssetType>::RequestAsset(
	const xml::Element* properties)
{
	std::string name = "";
	if( !xml::GetAttribute(properties, "name", name) )
		return AssetHandle();

	AssetHandle handle = container_->GetAssetHandle(name);
	if( !container_->IsHandelValid(handle) )
	{
		// we have to load the asset
		std::unique_ptr<AssetType> asset =
			AssetType::LoadAsset(asset_manager_, properties);
		if( !asset )
			return AssetHandle();

		handle = container_->AddAsset(std::move(asset));
		container_->IncrementAssetRefCount(handle);
		container_->RegisterAssetAlias(handle, name);
	}

	container_->IncrementAssetRefCount(handle);
	return handle;
}

template<typename AssetType>
inline bool SerialAssetLoader<AssetType>::ReleaseAsset(
	AssetHandle asset_handel)
{
	if( container_->IsHandelValid(asset_handel) )
	{
		container_->DecrementAssetRefCount(asset_handel);
		if( container_->GetAssetRefCount(asset_handel) == 0 )
		{
			AssetType::DeleteAsset(
				asset_manager_,
				container_->RemoveAsset(asset_handel));
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

} //end of namespace bembel
/*=================================<===========================================*/
/* END OF FILE                                                                */
/*============================================================================*/
