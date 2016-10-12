/*============================================================================*/
/* TEMPLATE IMPLEMENTATION                                                    */
/*============================================================================*/
namespace bembel{

template<typename AssetType>
inline SerialAssetLoader<AssetType>::SerialAssetLoader(
	AssetManager* assetMgr, 
	ContainerTypePtr container)
	: _assetMgr(assetMgr)
	, _container(container)
{}

template<typename AssetType>
inline SerialAssetLoader<AssetType>::~SerialAssetLoader()
{}

template<typename AssetType>
inline bool SerialAssetLoader<AssetType>::CreateAsset(
	const xml::Element* properties )
{
	std::string name = "";
	if (!xml::GetAttribute(properties, "name", name) ||
		_container->HasAsset(name))
	{
		// all assets must have a unique name
		return false;
	}

	std::unique_ptr<AssetType> asset = 
		AssetType::CreateAsset(_assetMgr, properties);
	if (asset == nullptr)
		return false; // failed to create asset

	AssetHandle handle = _container->AddAsset(std::move(asset));
	if (!_container->IsHandelValid(handle))
		return false;

	_container->RegisterAssetAlias(handle, name);

	for (const xml::Element* alias : xml::IterateChildElements(properties, "Alias"))
	{
		if (xml::GetAttribute(alias, "name", name))
			_container->RegisterAssetAlias(handle, name);
	}
	return true;
}

template<typename AssetType>
inline AssetHandle SerialAssetLoader<AssetType>::RequestAsset(
	const std::string& fileName )
{
	AssetHandle handle = _container->GetAssetHandle(fileName);

	if (_container->IsHandelValid(handle))
		return handle;// asset already loaded

	std::unique_ptr<AssetType> asset =
		AssetType::LoadFromFile(_assetMgr, fileName);
	if (!asset)
		return AssetHandle();

	handle = _container->AddAsset(std::move(asset));
	_container->RegisterAssetAlias(handle, fileName);
	return handle;
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
