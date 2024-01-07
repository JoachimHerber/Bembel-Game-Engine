#version 420
 
in DATA {
	vec3 tangent;
	vec3 binormal;  
	vec3 normal; 
	vec2 tex_coord;
} frag_data;

layout(std140) uniform Material
{
	vec4  emission;
	vec4  base_color;
	float roughness;
	float metallic;
	float f0;
} material;

layout(binding = 0) uniform sampler2D uEmissionMap;
layout(binding = 1) uniform sampler2D uBaseColorMap;
layout(binding = 2) uniform sampler2D uMaterialMap;
layout(binding = 3) uniform sampler2D uNormalMap;

layout(location = 0) out vec3 oEmission;
layout(location = 1) out vec3 oBaseColor;
layout(location = 2) out vec3 oMaterial;
layout(location = 3) out vec3 oNormal;

void main()
{
 	oEmission  = texture(uEmissionMap,  frag_data.tex_coord).rgb * material.emission.rgb; 
	oBaseColor = texture(uBaseColorMap, frag_data.tex_coord).rgb * material.base_color.rgb;
 	oMaterial  = texture(uMaterialMap,  frag_data.tex_coord).rgb * vec3(material.roughness, material.metallic, material.f0);
		
	vec3 normal = texture(uNormalMap, frag_data.tex_coord).rgb;
	normal      = 2.0*normal - vec3(1.0);
	normal      = normalize(mat3(normalize(frag_data.tangent), normalize(frag_data.binormal), normalize(frag_data.normal))*normal);
	oNormal     = 0.5*normal + vec3(0.5);
}
