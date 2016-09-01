#ifndef BEMBEL_ASSETCONTAINER_H
#define BEMBEL_ASSETCONTAINER_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include <BembelBase/Logging/Logger.h>

#include "AssetHandle.h"

#include <memory>
#include <vector>
#include <stack>
#include <map>

/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel{

class AssetContainerBase
{
public:
	AssetContainerBase()
	{};
	virtual ~AssetContainerBase()
	{};
};

template<typename AssetType>
class AssetContainer : public AssetContainerBase
{
public:
	AssetContainer();
	~AssetContainer();

	/**
	 * Adds an asset container to the container.
	 * @param[in] asset 
	 *	The asset that should be added to the container.
	 * @param[in] name 
	 *	A name that can later be used to query the asset. 
	 *	Can be empty.
	 *	If an empty name is specified, the handle for 
	 *	the asset can't be queried by GetAssetHandle().
	 * @return 
	 *	A handle that can be used to retrieve the asset.
	 */
	AssetHandle AddAsset(AssetType* asset, const std::string& name ="");

	/**
	 * removes an asset from the container
	 * @param[out] assetData
	 *	The data of the asset that is removed.
	 * @param[in] handle
	 *	The handle to the asset that should be removed.
	 * @param[in] force
	 *	If true, the asset will be removed even if its reference count is non-zero.
	 *	If false, the asset won't be removed if its reference count is non-zero.
	 * @return
	 *  True if an asset has been removed.
	 */
	AssetType* RemoveAsset(AssetHandle handle, bool force = false);

	/**
	 * Returns the a handle for the requested asset.
	 * @param[in] name
	 *	The name of the requested asset. 
	 *	The name of an asset is specified when the asset is added to the container.
	 */
	AssetHandle GetAssetHandle(const std::string& name);

	/**
	 * return true if the specified handle points to a valid asset.
	 */
	bool IsHandelValid(AssetHandle handle);

	bool HasAsset(const std::string& name)
	{
		return IsHandelValid(GetAssetHandle(name));
	}

	/**
	 * Returns the data of the asset specified by the handle or the data of a 
	 * dummy asset if the specified handle is infalid;
	 */
	AssetType* GetAsset(AssetHandle handle, bool returnDummyIfHandleInfalid = true);

	/**
	* Return the reference count for the asset specified by the handle.
	*/
	int GetAssetRefCount(AssetHandle handle);

	/**
	* Increments the reference count of the asset specified by the handle.
	*/
	void IncrementAssetRefCount(AssetHandle handle);

	/**
	 * Decrements the reference count of the asset specified by the handle.
	 */
	void DecrementAssetRefCount(AssetHandle handle);

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
	 * Specifies the asset data that is returned by GetAsset() when it
	 * is called with an invalid asset-handle.
	 */
	void SetDummyAsset(AssetHandle dummy);

protected:
	struct AssetData
	{
		AssetType*    data;
		std::size_t   hash;
		int           refCount;
	};

	std::map<std::string, AssetHandle> _assetMap;
	std::vector<AssetData>             _assets;

	std::stack<unsigned> _unusedIds;

	AssetHandle _dummyAsset;
};

template<typename T>
using AssetContainerPtr = std::shared_ptr<AssetContainer<T>>;

} //end of namespace bembel
/*============================================================================*/
/* INLINE IMPLEMENTATIONS													  */
/*============================================================================*/

#include "AssetContainer.inl"

/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
