#version 330

uniform sampler2D uDepthBuffer;
uniform sampler2D uNormalBuffer;
uniform sampler2D uAlbedoBuffer;

uniform mat4 uInverseProjectionMatrix;
uniform vec2 uTexelSize;

in vec3  vLightPos;
in vec3  vLightColor;
in float vBulbRadius;
in float vCutoffRadius;

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
	
	vec4 position = uInverseProjectionMatrix*vec4( 2*gl_FragCoord.xy*uTexelSize - vec2(1.0), 2*deapt - 1, 1 );
	position.xyz /= position.w;
		
	vec3 ligthDirection = position.xyz-vLightPos;
	float ligthDistance = length(ligthDirection);
	ligthDirection /= ligthDistance;

	float att = (1 + ligthDistance/vBulbRadius)/(1 + 1/vBulbRadius);
	att = smoothstep(vCutoffRadius, 0.8*vCutoffRadius, ligthDistance)/(att*att);

	oColor.rgb = vLightColor*att*BRDF(
		normalize(normal.xyz - vec3(0.5)),
		-ligthDirection, 
		-normalize(position.xyz), 
		mat);
	oColor.a   = 0;
}