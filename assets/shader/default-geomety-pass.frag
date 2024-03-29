#version 330
 
in DATA {
	vec3 normal;
	vec2 tex_coord;
} frag_data;


layout(std140) uniform Material
{
	vec3 emission;
	vec3 albedo;
	float roughness;
	float metallic;
	float f0;
} material;


layout(location = 0) out vec3 oEmission;
layout(location = 1) out vec3 oAlbedo;
layout(location = 2) out vec3 oMaterial;
layout(location = 3) out vec3 oNormal;

vec3 EncodeNormal(vec3 n)
{
	return 0.5*n + vec3(0.5);
}

void main()
{
 	oEmission    = material.emission;
	oAlbedo      = material.albedo;
 	oMaterial    = vec3(material.roughness, material.metallic, material.f0);
 	oNormal      = EncodeNormal(normalize(frag_data.normal));
}
