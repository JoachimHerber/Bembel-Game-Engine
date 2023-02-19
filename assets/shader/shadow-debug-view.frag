#version 330

layout(binding=0) uniform sampler2D uDepthBuffer;
uniform float uResulution;

in vec2 vTexCoord;

layout(location = 0) out vec3 oColor;

float linearize_depth(float d,float zNear,float zFar)
{
    return zNear * zFar / (zFar + d * (zNear - zFar));
}
void main()
{
	//float depth     = texelFetch( uDepthBuffer, ivec2(vTexCoord*uResulution), 0 ).r;
	float depth = texture( uDepthBuffer, vTexCoord).r;
	
	oColor = vec3(1-fract(100*depth), fract(linearize_depth(depth, 0.1, 100.0)),0);
}
