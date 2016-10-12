/*============================================================================*/
/* INLINE IMPLEMENTETION                                                      */
/*============================================================================*/
namespace bembel{

template<typename AssetType>
inline AssetContainer<AssetType>::AssetContainer(uint16_t typeId)
	: _assetTypeId(typeId)
{}

template<typename AssetType>
inline AssetContainer<AssetType>::~AssetContainer()
{}

template<typename AssetType>
inline AssetHandle AssetContainer<AssetType>::AddAsset(
	AssetPtr asset)
{
	AssetHandle handle;

	// find a place to put the asset
	if (!_unusedIds.empty())
	{
		handle.index = _unusedIds.top();
		handle.typeId = _assetTypeId;
		
		_unusedIds.pop();

		_assets[handle.index].asset = std::move(asset);
		_assets[handle.index].refCount = 0;
		handle.generation = ++_assets[handle.index].generation;
	}
	else
	{
		handle.index = _assets.size();
		handle.typeId = _assetTypeId;
		handle.generation = 1;

		_assets.push_back({std::move(asset), 1, 0});
	}
	return handle;
}

template<typename AssetType>
inline bool AssetContainer<AssetType>::RegisterAssetAlias(
	AssetHandle handle, const std::string& alias)
{
	if (_assetAliasses.find(alias) != _assetAliasses.end() )
		return false; // alias already in use

	if (!IsHandelValid(handle))
		return false; // invalid handle

	_assetAliasses.emplace(alias, handle);
}

template<typename AssetType>
inline std::unique_ptr<AssetType>
	AssetContainer<AssetType>::RemoveAsset(
	AssetHandle handle, bool force)
{
	if (IsHandelValid(handle))
		return nullptr; // invalid handle

	if (_assets[handle.index].refCount > 0)
	{
		if (force)
			BEMBEL_LOG_WARNING() << "Removing asset that is still in use." << std::endl;
		else
			return nullptr;
	}

	AssetPtr asset = std::move(_assets[handle.index].asset);
	_assets[handle.index].refCount = -1;
	_unusedIds.push(handle.index);

	return std::move(asset);
}

template<typename AssetType>
inline AssetHandle AssetContainer<AssetType>::GetAssetHandle(const std::string& name)
{
	auto it = _assetAliasses.find(name);
	if (it == _assetAliasses.end())
		return AssetHandle();

	if (IsHandelValid(it->second))
		return it->second;

	// The entry in the assetMap refers to an asset, which no longer exist.
	// The entry should hence be removed
	_assetAliasses.erase(it);
	return AssetHandle();
}

template<typename AssetType>
inline bool AssetContainer<AssetType>::IsHandelValid(AssetHandle handle)
{
	if (handle.typeId != _assetTypeId)
		return false;
	if (handle.index >= _assets.size())
		return false;
	if (handle.generation != _assets[handle.index].generation)
		return false;
	return _assets[handle.index].refCount >= 0;
}

template<typename AssetType>
inline AssetType* AssetContainer<AssetType>::GetAsset(
	AssetHandle handle, 
	bool returnDummyIfHandleInvalid)
{
	if (IsHandelValid(handle))
		return _assets[handle.index].asset.get();

	if (returnDummyIfHandleInvalid && IsHandelValid(_dummyAsset))
		return _assets[_dummyAsset.index].asset.get();

	return nullptr;
}

template<typename AssetType>
inline int AssetContainer<AssetType>::GetAssetRefCount(AssetHandle handle)
{
	if (!IsHandelValid(handle))
		return -1;

	return _assets[handle.index].refCount;
}

template<typename AssetType>
inline void AssetContainer<AssetType>::IncrementAssetRefCount(AssetHandle handle)
{
	if (IsHandelValid(handle))
		++_assets[handle.index].refCount;
}
template<typename AssetType>
inline void AssetContainer<AssetType>::DecrementAssetRefCount(AssetHandle handle)
{
	if (IsHandelValid(handle) && _assets[handle.index].refCount > 0)
		--_assets[handle.index].refCount;
}

template<typename AssetType>
inline void AssetContainer<AssetType>::GetAssets(std::vector<AssetHandle>& asset)
{
	asset.clear();
	asset.reserve(_assets.size());

	for (size_t n = 0; n < _assets.size(); ++n)
	{
		if (_assets[n].refCount >= 0)
			asset.push_back(AssetHandle{n, _assets[n].hash});
	}
}

template<typename AssetType>
inline void AssetContainer<AssetType>::GetUnusedAssets(
	std::vector<AssetHandle>& asset)
{
	asset.clear();
	asset.reserve(_assets.size());

	for (size_t n = 0; n < _assets.size(); ++n)
	{
		if (_assets[n].refCount == 0)
			asset.push_back(AssetHandle{n, _assets[n].hash});
	}
}

template<typename AssetType>
inline void AssetContainer<AssetType>::SetDummyAsset(
	AssetHandle dummy)
{
	_dummyAsset = dummy;
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/