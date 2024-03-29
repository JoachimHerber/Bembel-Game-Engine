#version 330

uniform int uState;
uniform float uTime;
uniform sampler3D uNoise;

in vec3 vNormal;
in vec4 vWorldPos;
in vec4 vRelPos;

const vec4 g_colors[3] = vec4[](
vec4(0.0, 1.0, 1.0, 0.7), 
vec4(1.0, 0.5, 0.2, 1.0),
vec4(1.0, 0.5, 0.0, 1.0));

layout(location = 0) out vec4 oColor;
 
void main()
{	
	float noise1 = texture(uNoise, 0.1*vWorldPos.xyz + 0.07*uTime*vec3( 0.0, 1.0, 0.5)).r - 0.5;
	float noise2 = texture(uNoise, 0.2*vWorldPos.xyz + 0.20*uTime*vec3( 0.4, 1.0, 0.3)).r - 0.5;
	float noise3 = texture(uNoise, 0.4*vWorldPos.xyz + 0.60*uTime*vec3(-0.4, 1.0, 0.3)).r - 0.5;
	float alpha = 0.9 + 0.4*dot(normalize(vNormal), normalize(vRelPos.xyz));
 	oColor = g_colors[uState%3];
	oColor.a *= alpha;
	oColor.rgb *= clamp(0.5*noise1 +  0.3*noise2 + 0.2*noise3 + 0.5, 0.0, 1.0);
}

