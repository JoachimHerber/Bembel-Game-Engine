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
	std::string path;

	auto& locator = asset_manager_->GetAssetLocator();
	if( !locator.FindAssetLocation<AssetType>(file_name, &path) )
	{
		// can't find the requested file
		return AssetHandle();
	}

	// test if the asset is alredy loaded
	AssetHandle handle = container_->GetAssetHandle(path);
	if( !container_->IsHandelValid(handle) )
	{
		// we have to load the asset
		std::unique_ptr<AssetType> asset =
			AssetType::LoadAsset(asset_manager_, path);
		if( !asset )
			return AssetHandle();

		handle = container_->AddAsset(std::move(asset));
		container_->RegisterAssetAlias(handle, path);
	}

	container_->IncrementAssetRefCount(handle);
	return handle;
}

template<typename AssetType>
inline AssetHandle SerialAssetLoader<AssetType>::RequestAsset(
	const xml::Element* properties)
{
	std::string name = "";
	if( xml::GetAttribute(properties, "name", name) )
	{
		AssetHandle handle = container_->GetAssetHandle(name);
		if( container_->IsHandelValid(handle) )
		{
			container_->IncrementAssetRefCount(handle);
			return handle;
		}
	}

	std::string file_name = "";
	if( xml::GetAttribute(properties, "file_name", file_name) )
	{
		AssetHandle handle = RequestAsset(file_name);
		if( container_->IsHandelValid(handle) && !name.empty() )
		{
			container_->RegisterAssetAlias(handle, name);
		}
		return handle;
	}

	std::unique_ptr<AssetType> asset =
			AssetType::CreateAsset(asset_manager_, properties);
	if( !asset )
		return AssetHandle();

	AssetHandle handle = container_->AddAsset(std::move(asset));
	container_->IncrementAssetRefCount(handle);
	if( !name.empty() )
		container_->RegisterAssetAlias(handle, name);

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
