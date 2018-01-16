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

#ifndef BEMBEL_KERNEL_ASSETS_ASSETMANAGER_H_
#define BEMBEL_KERNEL_ASSETS_ASSETMANAGER_H_
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "bembel-config.h"

#include "asset-container.h"
#include "asset-loader.h"
#include "asset-locator.h"

#include <cassert>
#include <string>
#include <vector>
#include <unordered_map>

#include <bembel-base/xml.h>
#include <bembel-base/logging/logger.h>

/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel {

class BEMBEL_API AssetManager final
{
public:
public:
	AssetManager();
	~AssetManager();

	/**
	 * Adds an asset.
	 * The added asset will be managed by the asset manager and its
	 * refernence count will be initalised to 1.
	 */
	template<typename AssetType>
	AssetHandle AddAsset(
		std::unique_ptr<AssetType> asset, const std::string& name = "");

	/**
	 * Request an asset.
	 * The requested asset will be loaded if it isn't loaded already.
	 * Requesting an asset will increse the reference count of the asset.
	 */
	template<typename AssetType>
	AssetHandle RequestAsset(const std::string& file_name);
	template<typename AssetType>
	AssetHandle RequestAsset(const xml::Element* properties);
	AssetHandle RequestAsset(
		const std::string& asset_type_name,
		const std::string& file_name);
	AssetHandle RequestAsset(
		const std::string& asset_type_name,
		const xml::Element* properties);

	/**
	 * Releses an asset.
	 * This metode will reduce the reference count of the asset and delete
	 * the asset once its reference count reches zero.
	 */
	bool ReleaseAsset(AssetHandle asset_handle);

	/**
	 * @return true if the asset_handle is valide.
	 */
	bool IsHandelValid(AssetHandle asset_handle);

	/**
	 * Queries a handle for a named asset.
	 * Does not modify the reference count of the asset.
	 * @return a handle to the asset with the request name if it exist.
	 */
	template<typename AssetType>
	AssetHandle GetAssetHandle(const std::string& name);

	/**
	 * returns a pointer to the specified asset or a pointer to a dummy asset.
	 */
	template<typename AssetType>
	AssetType* GetAsset(AssetHandle handle, bool return_dummy_if_handle_invalid = true);

	template<typename AssetType, typename ... TArgs>
	bool RegisterAssetType(TArgs ... args);
	template<typename AssetType, typename AssetLoaderType, typename ... TArgs>
	bool RegisterAssetType(TArgs ... args);

	AssetLocator& GetAssetLocator();

private:
	template<typename AssetType>
	AssetContainer<AssetType>* GetAssetContainer();

private:
	std::unordered_map<std::string, uint16_t>  asset_type_map_;

	std::vector<std::shared_ptr<AssetContainerBase>> asset_container_;
	std::vector<std::shared_ptr<AssetLoaderBase>>    asset_loader_;

	AssetLocator asset_locator_;
};

} //end of namespace bembel
/*============================================================================*/
/* TEMPLATE IMPLEMENTATION                                                    */
/*============================================================================*/

#include "asset-manager.inl"

/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
