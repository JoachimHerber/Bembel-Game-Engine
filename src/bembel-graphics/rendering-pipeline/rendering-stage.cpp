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
/* INCLUDES                                                                   */
/*============================================================================*/

#include "rendering-stage.h"
#include "rendering-pipeline.h"

#include <bembel-kernel/rendering/texture.h>
#include <bembel-kernel/rendering/frame-buffer-object.h>

/*============================================================================*/
/* IMPLEMENTATION			                                                  */
/*============================================================================*/
namespace bembel {

RenderingStage::RenderingStage(RenderingPipeline* pipline)
	: pipline_{pipline}
	, fbo_{std::make_unique<FrameBufferObject>()}
{}
RenderingStage::~RenderingStage()
{}

void RenderingStage::SetScene(Scene*)
{}

void RenderingStage::Init()
{
	fbo_->Init();
}

void RenderingStage::Cleanup()
{
	fbo_->CleanUp();
}

AssetManager* RenderingStage::GetAssetManager() const
{
	return pipline_->GetAssetManager();
}

void RenderingStage::SetInputTextures(
	const std::vector<std::string>& textures)
{
	textures_.clear();
	for( size_t n = 0; n < textures.size(); ++n )
	{
		textures_.push_back(pipline_->GetTexture(textures[n]));
	}
}

void RenderingStage::SetDepthOutputTexture(const std::string& texture)
{
	fbo_->SetDepthAttechment(pipline_->GetTexture(texture));
}

void RenderingStage::SetColorOutputTexture(
	unsigned index, const std::string& texture)
{
	fbo_->SetColorAttechment(index, pipline_->GetTexture(texture));
}

void RenderingStage::BindInputTextures()
{
	for( size_t n = 0; n<textures_.size(); ++n )
	{
		if( textures_[n] )
		{
			glActiveTexture(GL_TEXTURE0 + n);
			textures_[n]->Bind();
		}
	}
}

void RenderingStage::ReleaseInputTextures()
{
	for( size_t n = textures_.size(); n>0; --n )
	{
		if( textures_[n-1] )
		{
			glActiveTexture(GL_TEXTURE0 + n-1);
			textures_[n-1]->Release();
		}
	}
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
