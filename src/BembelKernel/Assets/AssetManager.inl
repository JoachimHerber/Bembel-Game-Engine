/*============================================================================*/
/* TEMPLATE IMPLEMENTATION                                                    */
/*============================================================================*/
namespace bembel{

template<typename AssetType>
inline std::shared_ptr<AssetContainer<AssetType>> 
	AssetManager::RequestAssetContainer()
{
	auto it = _assetTypeMap.find(AssetType::GetTypeName());
	if (it != _assetTypeMap.end())
	{
		return std::static_pointer_cast<AssetContainer<AssetType>>(
			_container[it->second]);
	}
	else
	{
		auto container = 
			std::make_shared<AssetContainer<AssetType>>();

		_assetTypeMap.emplace(
			AssetType::GetTypeName(), _container.size());
		_container.push_back(container);
		return container;
	}
}

template<typename AssetType>
inline std::shared_ptr<AssetContainer<AssetType>>
AssetManager::GetAssetContainer()
{
	return std::static_pointer_cast<AssetContainer<AssetType>>(
		GetAssetContainer(AssetType::GetTypeName()));
}

template<typename AssetType>
inline std::shared_ptr<AssetLoaderBase> AssetManager::GetAssetLoader()
{
	return GetAssetLoader(AssetType::GetTypeName());
}

template<typename AssetType, typename AssetLoaderType, typename ... TArgs>
inline bool AssetManager::InitAssetLoader(TArgs ... args)
{
	std::shared_ptr<AssetLoaderBase> loader = 
		AssetLoaderType::CreateDefaultLoader(this, args ...);
	if (!loader)
		return false;
	return SetAssetLoader(AssetType::GetTypeName(), loader);
}

template<typename AssetType>
inline AssetHandle AssetManager::GetAssetHandle(
	const std::string& name)
{
	auto container = GetAssetContainer<AssetType>();
	if (container)
		return container->GetAssetHandle(name);
	return AssetHandle{~0U,~0U};
}

template<typename AssetType>
inline AssetType* AssetManager::GetAsset(
	AssetHandle handle)
{
	auto container = GetAssetContainer<AssetType>();
	if (container)
		return container->GetAsset(handle);
	return nullptr;
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
