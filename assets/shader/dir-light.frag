#version 330
#extension GL_ARB_shading_language_420pack: enable

uniform vec3  uLigthColor;
uniform vec3  uLigthDir;
uniform int   uHasShadow;
uniform int   uMinShadowMapLayer;
uniform int   uNumCascades;
uniform float uCascadeDistances[64];

uniform mat4 uInverseViewMatrix;
uniform mat4 uInverseProjectionMatrix;

uniform mat4 uShadowMatrices[64];

layout(binding=0) uniform sampler2D uDepthBuffer;
layout(binding=1) uniform sampler2D uAlbedoBuffer;
layout(binding=2) uniform sampler2D uMaterialBuffer;
layout(binding=3) uniform sampler2D uNormalBuffer;

layout(binding=4) uniform sampler2DArray uShadowMap;

layout(pixel_center_integer) in vec4 gl_FragCoord;

in vec2 vTexCoord;

layout(location = 0) out vec3 oColor;

struct Material
{
  float roughness;
  vec3  diffuse_color;
  vec3  specular_color;
};

bool GetGeomData(vec2 texCoord, out vec4 position, out vec3 normal, out Material mat);
vec3 BRDF(vec3 n, vec3 l, vec3 v, Material mat);

vec2 POISSON_DISK[4] = vec2[](
  vec2( -0.94201624, -0.39906216 ),
  vec2( 0.94558609, -0.76890725 ),
  vec2( -0.094184101, -0.92938870 ),
  vec2( 0.34495938, 0.29387760 )
);

float  calculateShadow(vec4 fragPosViewSpace, int layer, float bias){
	vec4 fragPosLightSpace = uShadowMatrices[layer] * fragPosViewSpace;

	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	projCoords = projCoords * 0.5 + 0.5; 
	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(uShadowMap, 0).xy;

	float depthThreshhold = projCoords.z - bias;

	float visibility = texture(uShadowMap, vec3(projCoords.xy, layer)).r <  depthThreshhold ? 0.5 : 1.0;
	for (int i=0;i<4;i++){
		if ( texture( uShadowMap, vec3(projCoords.xy + POISSON_DISK[i]/700.0, layer)).r  <  depthThreshhold ){
			visibility-=0.2;
		}
	}
	return max(visibility, 0);
}

void main()
{
	vec4 position;
	vec3 normal;
	Material mat;
	if(GetGeomData(vTexCoord, position, normal, mat))
	{
		vec3 lightDir = normalize(uLigthDir);
		vec3 light = max(uLigthColor*BRDF(normal, -lightDir, -normalize(position.xyz), mat),vec3(0));
		
		float dist = abs(position.z);
		
		if(uHasShadow != 0){
			for(int i = 0; i < uNumCascades; ++i){
				if(dist < uCascadeDistances[i]){
					float bias = max(0.0005 * (1.0 - dot(normal, -lightDir)), 0.0); 
					light *= calculateShadow(uInverseViewMatrix* vec4(position.xyz, 1), uMinShadowMapLayer + i, bias);
					break;
				}
			}
		}
		oColor = light; 
	}
	else
	{
		oColor = vec3(0);
	}
}
