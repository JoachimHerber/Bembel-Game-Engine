/******************************************************************************/
/* ************************************************************************** */
/* *                                                                        * */
/* *    MIT License                                                         * */
/* *                                                                        * */
/* *   Copyright(c) 2017 Joachim Herber                                     * */
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
/* TEMPLATE IMPLEMENTATION                                                    */
/*============================================================================*/
namespace bembel {

template<typename AssetType>
inline AssetContainer<AssetType>*
AssetManager::GetAssetContainer()
{
	auto it = asset_type_map_.find(AssetType::GetTypeName());
	if( it == asset_type_map_.end() )
		return nullptr;

	return static_cast<AssetContainer<AssetType>*>(
		asset_container_[it->second].get());
}

template<typename AssetType>
inline AssetHandle AssetManager::AddAsset(
	std::unique_ptr<AssetType> asset,
	const std::string& name )
{
	auto container = GetAssetContainer<AssetType>();
	if( container )
	{
		auto handle = container->AddAsset(std::move(asset));
		container->IncrementAssetRefCount(handle);
		if(!name.empty())
			container->RegisterAssetAlias(handle, name);
		return handle;
	}
	return AssetHandle();
}

template<typename AssetType>
inline AssetHandle AssetManager::RequestAsset(const std::string& file_name)
{
	return RequestAsset(AssetType::GetTypeName(), file_name);
}
template<typename AssetType>
inline AssetHandle AssetManager::RequestAsset(const xml::Element* properties)
{
	return RequestAsset(AssetType::GetTypeName(), properties);
}

template<typename AssetType>
inline AssetHandle AssetManager::GetAssetHandle(
	const std::string& name)
{
	auto container = GetAssetContainer<AssetType>();
	if( container )
		return container->GetAssetHandle(name);
	return AssetHandle();
}

template<typename AssetType>
inline AssetType* AssetManager::GetAsset(
	AssetHandle handle,
	bool return_dummy_if_handle_invalid)
{
	auto container = GetAssetContainer<AssetType>();
	if( container )
		return container->GetAsset(handle, return_dummy_if_handle_invalid);
	return nullptr;
}

template<typename AssetType, typename ... TArgs>
inline bool AssetManager::RegisterAssetType(TArgs ... args)
{
	auto it = asset_type_map_.find(AssetType::GetTypeName());
	if( it != asset_type_map_.end() )
		return false; // Asset type has already been registered

	assert(asset_type_map_.size() < 0xFFFFLU &&
		"number of asset-types surpasses the maximum");
	uint16_t type_id = asset_type_map_.size();

	auto container = std::make_shared<AssetContainer<AssetType>>(type_id);
	auto loader = std::make_shared<AssetType::DefaultLoaderType>(this, container.get(), args...);

	asset_type_map_.emplace(AssetType::GetTypeName(), type_id);
	asset_container_.push_back(container);
	asset_loader_.push_back(loader);
	return true;
}

template<typename AssetType, typename AssetLoaderType, typename ... TArgs>
inline bool AssetManager::RegisterAssetType(TArgs ... args)
{
	auto it = asset_type_map_.find(AssetType::GetTypeName());
	if( it != asset_type_map_.end() )
		return false; // Asset type has already been registered

	assert(asset_type_map_.size() < 0xFFFFLU &&
		"number of asset-types surpasses the maximum");
	uint16_t type_id = _assetTypeMap.size();

	auto container = std::make_shared<AssetContainer<AssetType>>(type_id);
	auto loader = std::make_shared<AssetLoaderType>(this, container.get(), args...);

	_assetTypeMap.emplace(AssetType::GetTypeName(), type_id);
	_assetContainer.push_back(container);
	_assetLoader.push_back(loader);
	return true;
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
