#version 330

//uniform vec2 uTexelSize;

in vec3  vLightPos;
in vec3  vLightColor;
in float vBulbRadius;
in float vCutoffRadius;

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
	if(!GetGeomData(vTexCoord, position, normal, mat))
	{
		oColor = vec3(0);
		return;
	}
	
	vec3 ligthDirection = position.xyz-vLightPos;
	float ligthDistance = length(ligthDirection);
	ligthDirection /= ligthDistance;

	float att = (1 + ligthDistance/vBulbRadius)/(1 + 1/vBulbRadius);
	att = smoothstep(vCutoffRadius, 0.8*vCutoffRadius, ligthDistance)/(att*att);

	oColor.rgb = max(vLightColor*att*BRDF(normal, -ligthDirection, -normalize(position.xyz), mat),vec3(0));
}
