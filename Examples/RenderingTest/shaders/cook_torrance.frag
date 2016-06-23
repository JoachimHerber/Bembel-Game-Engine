#version 330

struct Material
{
  float roughness;
  float ior;
  float metallic;
  vec3  albedo;
};

const float pi = 3.14159265359;

vec3 BRDF(vec3 n, vec3 l, vec3 v, Material mat)
{
    vec3 h = normalize(l + v);
		
	float NdotL = clamp(dot(n,l),0,1);
	float NdotH = clamp(dot(n,h),0,1);
	float NdotV = clamp(dot(n,v),0,1);
	float VdotH = clamp(dot(v,h),0,1);// equalt to LdotH;
	
	// geometric attenuation
	float G = min(1.0, min((2*NdotH*NdotV)/VdotH, (2*NdotH*NdotL)/VdotH));
		
	// roughness (or: microfacet distribution function)
	// beckmann distribution function
	float NdotH2 = NdotH*NdotH;
	float m2  = mat.roughness*mat.roughness;
	float D = exp((NdotH2 - 1.0) / (m2 * NdotH2))/ ( pi * m2 * NdotH2*NdotH2);
	
	// fresnel
	// Schlick approximation
	vec3 F0 = vec3(abs ((1.0 - mat.ior) / (1.0 + mat.ior)));
	F0 = F0 * F0;
	F0 = mix(F0, mat.albedo.rgb, mat.metallic);

	vec3 F = F0 + (1-F0)*pow(1.0 - VdotH, 5.0);
	
	vec3 specular = (F*D*G)/(pi*NdotV*NdotL);
	vec3 diffuse  = mat.albedo*(1-F)*(1-mat.metallic) / pi;
	return NdotL*(specular + diffuse);
}