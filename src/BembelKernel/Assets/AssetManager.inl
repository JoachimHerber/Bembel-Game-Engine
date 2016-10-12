/*============================================================================*/
/* TEMPLATE IMPLEMENTATION                                                    */
/*============================================================================*/
namespace bembel{

template<typename AssetType>
inline std::shared_ptr<AssetContainer<AssetType>>
AssetManager::GetAssetContainer()
{
	auto it = _assetTypeMap.find(AssetType::GetTypeName());
	if (it == _assetTypeMap.end())
		return nullptr;

	return std::static_pointer_cast<AssetContainer<AssetType>>(
		_assetContainer[it->second]);
}

template<typename AssetType>
inline AssetHandle AssetManager::RequestAsset(const std::string& filename)
{
	auto it = _assetTypeMap.find(AssetType::GetTypeName());
	if (it == _assetTypeMap.end())
		return AssetHandle();

	return _assetLoader[it->second]->RequestAsset(filename);
}

template<typename AssetType>
inline AssetHandle AssetManager::GetAssetHandle(
	const std::string& name)
{
	auto container = GetAssetContainer<AssetType>();
	if (container)
		return container->GetAssetHandle(name);
	return AssetHandle();
}

template<typename AssetType>
inline AssetType* AssetManager::GetAsset(
	AssetHandle handle, 
	bool returnDummyIfHandleInvalid)
{
	auto container = GetAssetContainer<AssetType>();
	if (container)
		return container->GetAsset(handle, returnDummyIfHandleInvalid);
	return nullptr;
}

template<typename AssetType, typename AssetLoaderType>
inline bool AssetManager::RegisterAssetType()
{
	auto it = _assetTypeMap.find(AssetType::GetTypeName());
	if (it != _assetTypeMap.end())
		return false; // Asset type has already been registered

	assert(_assetTypeMap.size() < 0xFFFFLU &&
		   "number of asset-types surpasses the maximum");
	uint16_t typeID = _assetTypeMap.size();

	auto container = std::make_shared<AssetContainer<AssetType>>(typeID);
	auto loader    = std::make_shared<AssetLoaderType>( this, container);

	_assetTypeMap.emplace(AssetType::GetTypeName(), typeID);
	_assetContainer.push_back(container);
	_assetLoader.push_back(loader);
	return true;
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
