/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "TextureView.h"

#include "OpenGL/ShaderProgram.h"
#include "OpenGL/Texture.h"

#include <string>


/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel {

const static std::string RenderingPipelineResultViewVert = R"(
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
const static std::string RenderingPipelineResultViewFrag = R"(
#version 330
uniform sampler2D uColor;

in vec2 texCoord;

out vec4 fragColor;

void main()
{
	fragColor = texture(uColor, texCoord);
}
)";

TextureView::TextureView(TexturePtr color)
	: _color(color)
	, _min(0,0)
	, _max(1,1)
{}

TextureView::~TextureView()
{}

void TextureView::SetViewArea(
	const glm::vec2& min, const glm::vec2& max)
{
	_min = min;
	_max = max;
}

void TextureView::Init()
{
	_shader = std::make_shared<ShaderProgram>();
	_shader->AttachShader(GL_VERTEX_SHADER,   RenderingPipelineResultViewVert);
	_shader->AttachShader(GL_FRAGMENT_SHADER, RenderingPipelineResultViewFrag);
	_shader->Link();

	_shader->Use();
	glUniform1i(_shader->GetUniformLocation("uColor"), 0);
	_uniform = _shader->GetUniformLocation("uArea");
	glUseProgram(0);
}

void TextureView::Cleanup()
{
	_shader.reset();
}

void TextureView::Draw()
{
	_shader->Use();
	glUniform4f(_uniform, _min.x, _min.y, _max.x-_min.x, _max.y-_min.y);

	_color->Bind();
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	_color->Release();

	glUseProgram(0);
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
