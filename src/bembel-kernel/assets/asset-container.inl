/******************************************************************************/
/* ************************************************************************** */
/* *                                                                        * */
/* *    MIT License                                                         * */
/* *                                                                        * */
/* *   Copyright(c) 2018 Joachim Herber                                     * */
/* *                                                                        * */
/* *   Permission is hereby granted, free of charge, to any person          * */
/* *   obtaining copy of this software and associated documentation files   * */
/* *   (the "Software"), to deal in the Software without restriction,       * */
/* *   including without limitation the rights to use, copy, modify, merge, * */
/* *   publish, distribute, sublicense, and/or sell copies of the Software, * */
/* *   and to permit persons to whom the Software is furnished to do so,    * */
/* *   subject to the following conditions :                                * */
/* *                                                                        * */
/* *   The above copyright notice and this permission notice shall be       * */
/* *   included in all copies or substantial portions of the Software.      * */
/* *                                                                        * */
/* *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,      * */
/* *   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF   * */
/* *   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND                * */
/* *   NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS   * */
/* *   BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN   * */
/* *   ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN    * */
/* *   CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE     * */
/* *   SOFTWARE.                                                            * */
/* *                                                                        * */
/* ************************************************************************** */
/******************************************************************************/

/*============================================================================*/
/* INLINE IMPLEMENTETION                                                      */
/*============================================================================*/
namespace bembel {

template<typename AssetType>
inline AssetContainer<AssetType>::AssetContainer(uint16_t type_id)
	: asset_type_id_(type_id)
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
	if( !unused_ids_.empty() )
	{
		handle.index = unused_ids_.top();
		handle.type_id = asset_type_id_;

		unused_ids_.pop();

		assets_[handle.index].asset = std::move(asset);
		assets_[handle.index].reference_count = 0;
		handle.generation = ++assets_[handle.index].generation;
	}
	else
	{
		handle.index = assets_.size();
		handle.type_id = asset_type_id_;
		handle.generation = 1;

		assets_.push_back({std::move(asset), 1, 0});
	}
	return handle;
}

template<typename AssetType>
inline bool AssetContainer<AssetType>::RegisterAssetAlias(
	AssetHandle handle, const std::string& alias)
{
	if( asset_aliasses_.find(alias) != asset_aliasses_.end() )
		return false; // alias already in use

	if( !IsHandelValid(handle) )
		return false; // invalid handle

	asset_aliasses_.emplace(alias, handle);
}

template<typename AssetType>
inline std::unique_ptr<AssetType>
AssetContainer<AssetType>::RemoveAsset(
	AssetHandle handle, bool force)
{
	if( !IsHandelValid(handle) )
		return nullptr; // invalid handle

	if( assets_[handle.index].reference_count > 0 )
	{
		if( force )
		{
			BEMBEL_LOG_WARNING()
				<< "Removing asset that is still in use." << std::endl;
		}
		else
			return nullptr;
	}

	AssetPtr asset = std::move(assets_[handle.index].asset);
	assets_[handle.index].reference_count = -1;
	unused_ids_.push(handle.index);

	return std::move(asset);
}

template<typename AssetType>
inline AssetHandle AssetContainer<AssetType>::GetAssetHandle(
	const std::string& name)
{
	auto it = asset_aliasses_.find(name);
	if( it == asset_aliasses_.end() )
		return AssetHandle();

	if( IsHandelValid(it->second) )
		return it->second;

	// The entry in the assetMap refers to an asset, which no longer exist.
	// The entry should hence be removed
	asset_aliasses_.erase(it);
	return AssetHandle();
}

template<typename AssetType>
inline bool AssetContainer<AssetType>::IsHandelValid(AssetHandle handle)
{
	if( handle.type_id != asset_type_id_ )
		return false;
	if( handle.index >= assets_.size() )
		return false;
	if( handle.generation != assets_[handle.index].generation )
		return false;
	return assets_[handle.index].reference_count >= 0;
}

template<typename AssetType>
inline AssetType* AssetContainer<AssetType>::GetAsset(
	AssetHandle handle,
	bool return_dummy_if_handle_invalid)
{
	if( IsHandelValid(handle) )
		return assets_[handle.index].asset.get();

	if( return_dummy_if_handle_invalid && IsHandelValid(dummy_asset_) )
		return assets_[dummy_asset_.index].asset.get();

	return nullptr;
}

template<typename AssetType>
inline int AssetContainer<AssetType>::GetAssetRefCount(AssetHandle handle)
{
	if( !IsHandelValid(handle) )
		return -1;

	return assets_[handle.index].reference_count;
}

template<typename AssetType>
inline void AssetContainer<AssetType>::IncrementAssetRefCount(AssetHandle handle)
{
	if( IsHandelValid(handle) )
		++assets_[handle.index].reference_count;
}
template<typename AssetType>
inline void AssetContainer<AssetType>::DecrementAssetRefCount(AssetHandle handle)
{
	if( IsHandelValid(handle) && assets_[handle.index].reference_count > 0 )
		--assets_[handle.index].reference_count;
}

template<typename AssetType>
inline void AssetContainer<AssetType>::GetAssets(std::vector<AssetHandle>& asset)
{
	asset.clear();
	asset.reserve(assets_.size());

	for( size_t n = 0; n < assets_.size(); ++n )
	{
		if( assets_[n].reference_count >= 0 )
			asset.push_back(AssetHandle{n, assets_[n].generation});
	}
}

template<typename AssetType>
inline void AssetContainer<AssetType>::GetUnusedAssets(
	std::vector<AssetHandle>& asset)
{
	asset.clear();
	asset.reserve(_assets.size());

	for( size_t n = 0; n < assets_.size(); ++n )
	{
		if( _assets[n].reference_count == 0 )
			asset.push_back(AssetHandle{n, assets_[n].generation});
	}
}

template<typename AssetType>
inline void AssetContainer<AssetType>::SetDummyAsset(
	AssetHandle dummy)
{
	dummy_asset_ = dummy;
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/