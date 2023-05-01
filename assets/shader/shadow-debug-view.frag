#version 330
#extension GL_ARB_shading_language_420pack: enable

layout(binding=0) uniform sampler2DArray uDepthBuffer;
uniform float uResulution;
uniform int   uLayer;

in vec2 vTexCoord;

layout(location = 0) out vec3 oColor;

void main()
{
	float depth     = texture( uDepthBuffer, vec3(vTexCoord, uLayer) ).r;
	//float depth = texture( uDepthBuffer, vTexCoord).r;
	
	oColor = vec3(1-fract(10*depth));
}
