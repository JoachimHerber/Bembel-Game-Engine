#version 330
 
in vec3 vNormal;
in vec2 vTexCoord;
 
uniform vec3 uAlbedo;
uniform vec3 uEmission;
uniform vec3 uReflectivity;
uniform float uRoughness;
 
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
 	oEmission     = uEmission;
 	oAlbedo       = uAlbedo;
 	oReflectivity  = uReflectivity;
 	oNormal       = vec4(EncodeNormal(normalize(vNormal)), uRoughness);
}