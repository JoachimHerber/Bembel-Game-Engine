#version 330
 
in vec3 vNormal;
in vec2 vTexCoord;

layout(std140) uniform Material
{
	vec4 uAlbedo;
	vec4 uEmission;
	vec4 uReflectivity;
	float uRoughness;
};
 
layout(location = 0) out vec3 oEmission;
layout(location = 1) out vec3 oAlbedo;
layout(location = 2) out vec3 oReflectivity;
layout(location = 3) out vec4 oNormal;

vec3 EncodeNormal (vec3 n)
{
	return 0.5*n + vec3(0.5);
}

void main()
{
 	oEmission     = uEmission.rgb;
 	oAlbedo       = uAlbedo.rgb;
 	oReflectivity  = uReflectivity.rgb;
 	oNormal       = vec4(EncodeNormal(normalize(vNormal)), uRoughness);
}