/*============================================================================*/
/* INLINE IMPLEMENTETION                                                      */
/*============================================================================*/
namespace bembel{

template<typename AssetType>
inline AssetContainer<AssetType>::AssetContainer()
{}

template<typename AssetType>
inline AssetContainer<AssetType>::~AssetContainer()
{
	for (auto& it :_assets)
	{
		delete it.data;
	}
}

template<typename AssetType>
inline AssetHandle AssetContainer<AssetType>::AddAsset(
	AssetType* data,
	const std::string&   name)
{
	AssetHandle handle = AssetHandle{~0U,~0U};

	if (!name.empty())
	{
		// a name has been specified for the for the asset so we
		// have to test if the specified name is already taken.
		if(IsHandelValid(GetAssetHandle(name)))
			return handle; // Asset already exists
	}

	handle.hash = std::hash<AssetType*>()(data);

	// find a place to put the asset
	if (!_unusedIds.empty())
	{
		handle.index = _unusedIds.top();
		_unusedIds.pop();

		_assets[handle.index].data = data;
		_assets[handle.index].hash = handle.hash;
		_assets[handle.index].refCount = 0;
	}
	else
	{
		handle.index = _assets.size();
		_assets.push_back({data, handle.hash, 0});
	}

	// add name to map if named
	if (!name.empty())
		_assetMap.emplace(name, handle);

	return handle;
}

template<typename AssetType>
inline AssetType* AssetContainer<AssetType>::RemoveAsset(
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

	AssetType* asset = _assets[handle.index].data;

	_assets[handle.index].data = nullptr;
	_assets[handle.index].refCount = -1;
	_unusedIds.push(handle.index);

	return asset;
}

template<typename AssetType>
inline AssetHandle AssetContainer<AssetType>::GetAssetHandle(const std::string& name)
{
	auto it = _assetMap.find(name);
	if (it == _assetMap.end())
		return AssetHandle{~0U,~0U};

	if (IsHandelValid(it->second))
		return it->second;

	// The entry in the assetMap refers to an asset, which no longer exist.
	// The entry should hence be removed
	_assetMap.erase(it);
	return AssetHandle{~0U,~0U};
}

template<typename AssetType>
inline bool AssetContainer<AssetType>::IsHandelValid(AssetHandle handle)
{
	if (handle.index >= _assets.size())
		return false;
	if (handle.hash != _assets[handle.index].hash)
		return false;
	return _assets[handle.index].refCount >= 0;
}

template<typename AssetType>
inline AssetType* AssetContainer<AssetType>::GetAsset(
	AssetHandle handle, 
	bool returnDummyIfHandleInfalid)
{
	if (IsHandelValid(handle))
		return _assets[handle.index].data;

	if (returnDummyIfHandleInfalid && IsHandelValid(_dummyAsset))
		return _assets[_dummyAsset.index].data;

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