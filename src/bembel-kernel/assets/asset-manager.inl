/*============================================================================*/
/* TEMPLATE IMPLEMENTATION                                                    */
/*============================================================================*/
namespace bembel{

template<typename AssetType>
inline AssetContainer<AssetType>*
AssetManager::GetAssetContainer()
{
	auto it = _assetTypeMap.find(AssetType::GetTypeName());
	if (it == _assetTypeMap.end())
		return nullptr;

	return static_cast<AssetContainer<AssetType>*>(
		_assetContainer[it->second].get());
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
inline AssetHandle AssetManager::RequestAsset( const xml::Element* properties )
{
	auto it = _assetTypeMap.find( AssetType::GetTypeName() );
	if( it == _assetTypeMap.end() )
		return AssetHandle();

	return _assetLoader[it->second]->RequestAsset( properties );
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

template<typename AssetType, typename ... TArgs>
inline bool AssetManager::RegisterAssetType( TArgs ... args )
{
	auto it = _assetTypeMap.find( AssetType::GetTypeName() );
	if( it != _assetTypeMap.end() )
		return false; // Asset type has already been registered

	assert( _assetTypeMap.size() < 0xFFFFLU &&
		"number of asset-types surpasses the maximum" );
	uint16_t typeID = _assetTypeMap.size();

	auto container = std::make_shared<AssetContainer<AssetType>>( typeID );
	auto loader = std::make_shared<AssetType::DefaultLoaderType>( this, container.get(), args... );

	_assetTypeMap.emplace( AssetType::GetTypeName(), typeID );
	_assetContainer.push_back( container );
	_assetLoader.push_back( loader );
	return true;
}

template<typename AssetType, typename AssetLoaderType, typename ... TArgs>
inline bool AssetManager::RegisterAssetType( TArgs ... args )
{
	auto it = _assetTypeMap.find(AssetType::GetTypeName());
	if (it != _assetTypeMap.end())
		return false; // Asset type has already been registered

	assert(_assetTypeMap.size() < 0xFFFFLU &&
		   "number of asset-types surpasses the maximum");
	uint16_t typeID = _assetTypeMap.size();

	auto container = std::make_shared<AssetContainer<AssetType>>(typeID);
	auto loader    = std::make_shared<AssetLoaderType>( this, container.get(), args... );

	_assetTypeMap.emplace(AssetType::GetTypeName(), typeID);
	_assetContainer.push_back(container);
	_assetLoader.push_back(loader);
	return true;
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
