#version 330

uniform sampler2D uDepthBuffer;
uniform sampler2D uAlbedoBuffer;
uniform sampler2D uReflectivityBuffer;
uniform sampler2D uNormalBuffer;

uniform mat4 uInverseProjectionMatrix;

layout(pixel_center_integer) in vec4 gl_FragCoord;

struct Material
{
  float roughness;
  vec3  albedo;
  vec3  F0;
};

bool GetGeomData(vec2 texCoord, out vec4 position, out vec3 normal, out Material mat)
{
	float deapt = texelFetch( uDepthBuffer, ivec2(gl_FragCoord.xy), 0 ).r;
	mat.albedo  = texelFetch( uAlbedoBuffer, ivec2(gl_FragCoord.xy), 0 ).rgb;
	mat.F0     = texelFetch( uReflectivityBuffer, ivec2(gl_FragCoord.xy), 0 ).rgb;
	vec4 n     = texelFetch( uNormalBuffer, ivec2(gl_FragCoord.xy), 0 );
	
	if(deapt==1)
		return true;
	
	position = uInverseProjectionMatrix*vec4( 2*texCoord - vec2(1.0), 2*deapt - 1, 1 );
	position.xyz /= position.w;

	normal = normalize(n.xyz - vec3(0.5));
	
	mat.roughness = n.a;
	return false;
}