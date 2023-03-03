#version 330
#extension GL_ARB_shading_language_420pack: enable

layout(binding=0) uniform sampler2D uDepthBuffer;
uniform float uResulution;

in vec2 vTexCoord;

layout(location = 0) out vec3 oColor;

void main()
{
	float depth     = texelFetch( uDepthBuffer, ivec2(vTexCoord*800), 0 ).r;
	//float depth = texture( uDepthBuffer, vTexCoord).r;
	
	oColor = vec3(1-fract(100*depth));
}
