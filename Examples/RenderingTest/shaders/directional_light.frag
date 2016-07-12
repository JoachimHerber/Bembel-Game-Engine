#version 330

uniform sampler2D uDepthBuffer;
uniform sampler2D uNormalBuffer;
uniform sampler2D uAlbedoBuffer;

uniform mat4 uInverseProjectionMatrix;

uniform vec3 uLigthColor;
uniform vec3 uLigthDir;

in vec2 vTexCoord;

layout(pixel_center_integer) in vec4 gl_FragCoord;

layout(location = 0) out vec4 oColor;

struct Material
{
  float roughness;
  float ior;
  float metallic;
  vec3  albedo;
};

vec3 BRDF(vec3 n, vec3 l, vec3 v, Material mat);

void main()
{
	float deapt  = texelFetch( uDepthBuffer,  ivec2(gl_FragCoord.xy), 0 ).r;
	vec4  normal = texelFetch( uNormalBuffer, ivec2(gl_FragCoord.xy), 0 );
	vec4  albedo = texelFetch( uAlbedoBuffer, ivec2(gl_FragCoord.xy), 0 );
	
	if(deapt==1)
	{
		oColor = vec4(0);
		return;
	}
	
	Material mat;
	mat.roughness = normal.w;
	mat.ior      = 1.2;
	mat.metallic  = 0 ;
	mat.albedo    = albedo.rgb;
	
	vec4 position = uInverseProjectionMatrix*vec4( 2*vTexCoord - vec2(1.0), 2*deapt - 1, 1 );
	position.xyz /= position.w;

	oColor.rgb = uLigthColor*BRDF(
		normalize(normal.xyz - vec3(0.5)),
		-normalize(uLigthDir), 
		-normalize(position.xyz), 
		mat);
	oColor.a   = 0;
}