/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "texture-view.h"

#include <string>

#include <bembel-kernel/rendering/shader.h>
#include <bembel-kernel/rendering/texture.h>

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel {

namespace {
const static std::string kRenderingPipelineResultViewVert = R"(
#version 330
uniform vec4 uArea;

out vec2 texCoord;

const vec2 g_vectices[4] =  vec2[](vec2(-1,-1), vec2(+1,-1), vec2(-1,+1), vec2(+1,+1));
const vec2 g_texCoords[4] = vec2[](vec2(0,0), vec2(1,0), vec2(0,1), vec2(1,1));

void main()
{
	gl_Position = vec4( g_vectices[gl_VertexID], 0, 1);
	texCoord    = g_texCoords[gl_VertexID]*uArea.zw + uArea.xy;
}
)";
const static std::string kRenderingPipelineResultViewFrag = R"(
#version 330
uniform sampler2D uColor;

in vec2 texCoord;

out vec4 fragColor;

void main()
{
	fragColor = texture(uColor, texCoord);
}
)";
} // anon namespace

TextureView::TextureView(AssetManager* asset_manager, Texture* texture)
	: asset_manager_{asset_manager}
	, texture_(texture)
	, min_(0, 0)
	, max_(1, 1)
{}

TextureView::~TextureView()
{}

void TextureView::SetViewArea(
	const glm::vec2& min, const glm::vec2& max)
{
	min_ = min;
	max_ = max;
}

void TextureView::Init()
{
	shader_ = new ShaderProgram();
	shader_->AttachShader(
		asset_manager_,
		asset_manager_->AddAsset<Shader>(Shader::CreateShader(
			GL_VERTEX_SHADER, kRenderingPipelineResultViewVert
	)));

	shader_->AttachShader(
		asset_manager_, 
		asset_manager_->AddAsset<Shader>(Shader::CreateShader(
			GL_FRAGMENT_SHADER, kRenderingPipelineResultViewFrag
	)));
	shader_->Link();

	shader_->Use();
	glUniform1i(shader_->GetUniformLocation("uColor"), 0);
	uniform_ = shader_->GetUniformLocation("uArea");
	glUseProgram(0);
}

void TextureView::Cleanup()
{
	delete shader_;
	shader_ = nullptr;
}

void TextureView::Draw()
{
	shader_->Use();
	glUniform4f(uniform_, min_.x, min_.y, max_.x-min_.x, max_.y-min_.y);

	texture_->Bind();
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	texture_->Release();

	glUseProgram(0);
}


const glm::vec2& TextureView::GetViewAreaMin() const
{
	return min_;
}

const glm::vec2& TextureView::GetViewAreaMax() const
{
	return max_;
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
