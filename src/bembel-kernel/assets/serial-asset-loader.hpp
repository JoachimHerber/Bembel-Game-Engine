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

#ifndef BEMBEL_KERNEL_ASSETS_SERIALASSETLOADER_H_
#define BEMBEL_KERNEL_ASSETS_SERIALASSETLOADER_H_
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "bembel-config.h"

#include "asset-loader.h"
#include "asset-manager.h"
#include "asset-locator.h"

/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel {

template<typename AssetType>
class SerialAssetLoader : public AssetLoaderBase
{
public:
	using ContainerType = AssetContainer<AssetType>;

	SerialAssetLoader(AssetManager* asset_manager, ContainerType* container);
	virtual ~SerialAssetLoader();

	virtual AssetHandle RequestAsset(const std::string& file_name) override;
	virtual AssetHandle RequestAsset(const xml::Element* properties) override;
	virtual bool ReleaseAsset(AssetHandle asset_handel) override;

	virtual void Update() override;

	virtual void  ResetProgess() override;
	virtual float GetProgress() override;
	virtual bool  LoadingFinished() override;

protected:
	AssetManager*   asset_manager_;
	ContainerType*  container_;
};

} //end of namespace bembel
/*============================================================================*/
/* TEMPLATE IMPLEMENTATION                                                    */
/*============================================================================*/

#include "serial-asset-loader.inl"

/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
