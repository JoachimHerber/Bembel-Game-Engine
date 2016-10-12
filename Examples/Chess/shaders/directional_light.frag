#version 330

uniform vec3 uLigthColor;
uniform vec3 uLigthDir;

in vec2 vTexCoord;

layout(location = 0) out vec3 oColor;

struct Material
{
  float roughness;
  vec3  albedo;
  vec3  F0;
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
		oColor = vec3(0);
		return;
	}
	
	oColor = uLigthColor*BRDF(normal, -normalize(uLigthDir), -normalize(position.xyz), mat);
}