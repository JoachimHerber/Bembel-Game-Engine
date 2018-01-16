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

#ifndef BEMBEL_GRAPHICS_GEOMETRY_MATERIAL_H_
#define BEMBEL_GRAPHICS_GEOMETRY_MATERIAL_H_
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "bembel-open-gl.h"
#include "bembel-config.h"

#include <vector>

#include <glm/glm.hpp>

#include <bembel-kernel/assets/asset-manager.h>
#include <bembel-kernel/assets/asset-loader.h>
#include <bembel-kernel/assets/asset-handle.h>

/*============================================================================*/
/* FORWARD DECLARATIONS                                                       */
/*============================================================================*/
namespace bembel {

class MaterialLoader;
class GraphicSystem;

}//end of namespace bembel
/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel {

class BEMBEL_API Material final
{
public:
	using DefaultLoaderType = MaterialLoader;

public:
	Material( unsigned renderer, GLint );
	~Material();

	unsigned GetRenderer() const;
	GLuint GetUniformBufferObject() const;

	const std::vector<AssetHandle>& GetTextures() const;
	void SetTextures(const std::vector<AssetHandle>& textures);

	const static std::string& GetTypeName();

private:
	unsigned renderer_ = 0;

	GLuint uniform_buffer_object_;

	std::vector<AssetHandle> textures_;
}; 

class BEMBEL_API MaterialLoader final : public AssetLoaderBase
{
public:
	using ContainerType = AssetContainer<Material>;

	MaterialLoader( AssetManager* , ContainerType*, GraphicSystem* );
	virtual ~MaterialLoader();

	virtual AssetHandle RequestAsset( const std::string& filename ) override;
	virtual AssetHandle RequestAsset( const xml::Element* properties ) override;
	virtual bool ReleaseAsset( AssetHandle assetHandel ) override;

	virtual void Update() override;

	virtual void  ResetProgess()    override;
	virtual float GetProgress()     override;
	virtual bool  LoadingFinished() override;

private:
	GraphicSystem*   graphic_system_;
	AssetManager*    asset_manager_;
	ContainerType*   container_;
};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
