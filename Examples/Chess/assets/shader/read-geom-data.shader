<Shader type="GL_FRAGMENT_SHADER">
#version 330
#extension GL_ARB_shading_language_420pack: enable

layout(binding=0) uniform sampler2D uDepthBuffer;
layout(binding=1) uniform sampler2D uAlbedoBuffer;
layout(binding=2) uniform sampler2D uReflectivityBuffer;
layout(binding=3) uniform sampler2D uNormalBuffer;

uniform mat4 uInverseProjectionMatrix;

layout(pixel_center_integer) in vec4 gl_FragCoord;

struct Material
{
  float roughness;
  vec3  diffuse_color;
  vec3  specular_color;
};

vec3 DecodeNormal(vec3 n)
{
    return normalize(2*n - vec3(1));
}

bool GetGeomData(vec2 texCoord, out vec4 position, out vec3 normal, out Material mat)
{
	float deapt      = texelFetch( uDepthBuffer, ivec2(gl_FragCoord.xy), 0 ).r;
	mat.diffuse_color  = texelFetch( uAlbedoBuffer, ivec2(gl_FragCoord.xy), 0 ).rgb;
	mat.specular_color = texelFetch( uReflectivityBuffer, ivec2(gl_FragCoord.xy), 0 ).rgb;
	vec4 n          = texelFetch( uNormalBuffer, ivec2(gl_FragCoord.xy), 0 );
	
	position = uInverseProjectionMatrix*vec4( 2*texCoord - vec2(1.0), 2*deapt - 1, 1 );
	position.xyz /= position.w;
	
	if(deapt==1)
		return false;
	
	normal = DecodeNormal(n.xyz);
	
	mat.roughness = n.a;
	return true;
}
</Shader>