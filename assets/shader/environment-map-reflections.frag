#version 330
#extension GL_ARB_shading_language_420pack: enable
 
uniform mat4 uInverseProjectionMatrix;
uniform mat4 uInverseViewMatrix;

layout(binding=4) uniform samplerCube uEnvironmentMap;

in vec2 vTexCoord;

const float PI = 3.14159265359;

layout(location = 0) out vec3 oColor;

struct Material
{
  float roughness;
  vec3  diffuse_color;
  vec3  specular_color;
};
bool GetGeomData(vec2 texCoord, out vec4 position, out vec3 normal, out Material mat);

vec3 Fresnel(float cos_theta, vec3 F0)
{
	return mix(vec3(pow(1.0 - cos_theta, 5.0)), vec3(1.0), F0); 
}

void main()
{
	vec4 position;
	vec3 normal;
	Material mat;
	if(GetGeomData(vTexCoord, position, normal, mat))
	{
		vec3 v = normalize(-position.xyz);
		vec3 l = normalize(-reflect(v, normal));
		
		float cos_theta = max(dot(normal, l),0);
	
		vec3  F = Fresnel(cos_theta, mat.specular_color);
		
		oColor = cos_theta*F*textureLod(uEnvironmentMap, 
			(uInverseViewMatrix*vec4(l,0)).xyz, 8*mat.roughness).rgb;
	}
	else
	{
		vec3 viewDir = (uInverseViewMatrix*vec4(normalize(position.xyz),0)).xyz;
		oColor = texture(uEnvironmentMap, viewDir).rgb;
	}
}
