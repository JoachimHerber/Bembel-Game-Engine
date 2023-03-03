#version 330
#extension GL_ARB_shading_language_420pack: enable

uniform vec3 uLigthColor;
uniform vec3 uLigthDir;
uniform int  uHasShadow;
uniform mat4 uShadowMatrix;
uniform mat4 uInverseViewMatrix;
uniform mat4 uInverseProjectionMatrix;

layout(binding=0) uniform sampler2D uDepthBuffer;
layout(binding=1) uniform sampler2D uAlbedoBuffer;
layout(binding=2) uniform sampler2D uMaterialBuffer;
layout(binding=3) uniform sampler2D uNormalBuffer;
layout(binding=4) uniform sampler2D uShadowMap;

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

void main()
{
	vec4 position;
	vec3 normal;
	Material mat;
	if(GetGeomData(vTexCoord, position, normal, mat))
	{
		vec3 lightDir = normalize(uLigthDir);
		vec3 light = max(uLigthColor*BRDF(normal, -lightDir, -normalize(position.xyz), mat),vec3(0));
		
		if(uHasShadow != 0){
			vec4 fragPosLightSpace = uShadowMatrix * uInverseViewMatrix* vec4(position.xyz, 1);
			vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
			projCoords = projCoords * 0.5 + 0.5; 
			float closestDepth = texture(uShadowMap, projCoords.xy).r;
			float currentDepth = projCoords.z; 
			
			float bias = max(0.0005 * (1.0 - dot(normal, -lightDir)), 0.0);  
			light *= currentDepth + bias > closestDepth  ? 0.0 : 1.0; 
		}
		oColor = light; 
	}
	else
	{
		oColor = vec3(0);
	}
}
