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

#ifndef BEMBEL_KERNEL_ASSETS_ASSETCONTAINER_H_
#define BEMBEL_KERNEL_ASSETS_ASSETCONTAINER_H_
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "asset-handle.h"

#include <memory>
#include <vector>
#include <stack>
#include <map>

#include <bembel-base/logging/logger.h>

/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel {

class AssetContainerBase
{
public:
	AssetContainerBase()
	{};
	virtual ~AssetContainerBase()
	{};

	virtual bool IsHandelValid(AssetHandle handle) = 0;

	virtual int GetAssetRefCount(AssetHandle handle) = 0;
	virtual void IncrementAssetRefCount(AssetHandle) = 0;
	virtual void DecrementAssetRefCount(AssetHandle) = 0;
};

template<typename AssetType>
class AssetContainer : public AssetContainerBase
{
public:
	using AssetPtr = std::unique_ptr<AssetType>;
public:
	AssetContainer(uint16_t typeId);
	~AssetContainer();

	/**
	 * Adds an asset container to the container.
	 * @param[in] asset
	 *	The asset that should be added to the container.
	 * @return
	 *	A handle that can be used to retrieve the asset.
	 */
	AssetHandle AddAsset(AssetPtr asset);

	/**
	 * Registers an alias for an asset.
	 * The alias can than be used to to restive the asset handle.
	 * @param[in] asset
	 *  The handle that specifies the asset.
	 * @param[in] handle
	 *  The alias that should be used for the asset.
	 * @return
	 *	true if successful.
	 *	false it handle is invalid or alias is already in use.
	 */
	bool RegisterAssetAlias(AssetHandle handle, const std::string& alias);

	/**
	 * removes an asset from the container
	 * @param[in] handle
	 *	The handle to the asset that should be removed.
	 * @param[in] force
	 *	If true, the asset will be removed even if its reference count is non-zero.
	 *	If false, the asset won't be removed if its reference count is non-zero.
	 * @return
	 *  True if an asset has been removed.
	 */
	AssetPtr RemoveAsset(AssetHandle handle, bool force = false);

	/**
	 * Returns the a handle for the requested asset.
	 * @param[in] name
	 *	The name of the requested asset.
	 *	The 'name' must have been register as an alias for the asset (see RegisterAssetAlias).
	 */
	AssetHandle GetAssetHandle(const std::string& name);

	/**
	 * return true if the specified handle points to a valid asset.
	 */
	bool IsHandelValid(AssetHandle handle) override;

	/**
	 * returns true if the specified name has been register as an alias for an asset.
	 */
	bool HasAsset(const std::string& name)
	{
		return IsHandelValid(GetAssetHandle(name));
	}

	/**
	 * Returns the asset specified by the handle or a dummy asset if the specified handle is infalid;
	 */
	AssetType* GetAsset(AssetHandle handle, bool return_dummy_if_handle_invalid = true);

	/**
	* Return the reference count for the asset specified by the handle.
	*/
	int GetAssetRefCount(AssetHandle handle) override;

	/**
	* Increments the reference count of the asset specified by the handle.
	*/
	void IncrementAssetRefCount(AssetHandle handle) override;

	/**
	 * Decrements the reference count of the asset specified by the handle.
	 */
	void DecrementAssetRefCount(AssetHandle handle) override;

	/**
	 * Fills the specified vector with handles to all assets contained in this container.
	 */
	void GetAssets(std::vector<AssetHandle>& asset);

	/**
	 * Fills the specified vector with handles to all assets contained
	 * in this container for which the reference count is zero.
	 */
	void GetUnusedAssets(std::vector<AssetHandle>& asset);

	/**
	 * Specifies the asset that is returned by GetAsset() when it
	 * is called with an invalid asset-handle.
	 */
	void SetDummyAsset(AssetHandle dummy);

protected:
	struct AssetData
	{
		AssetPtr asset;
		uint32_t generation;
		int32_t reference_count;
	};

	uint16_t asset_type_id_;

	std::map<std::string, AssetHandle> asset_aliasses_;

	std::vector<AssetData> assets_;
	std::stack<unsigned>   unused_ids_;

	AssetHandle dummy_asset_;
};

} //end of namespace bembel
/*============================================================================*/
/* INLINE IMPLEMENTATIONS													  */
/*============================================================================*/

#include "asset-container.inl"

/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
