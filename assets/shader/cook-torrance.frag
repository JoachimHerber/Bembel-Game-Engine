#version 330

struct Material
{
  float roughness;
  vec3  diffuse_color;
  vec3  specular_color;
};

const float kPI = 3.14159265359;

//             N 
//             /\`-._ 
//            /  \ 
//                \        _____ 
//  L ___         _\.----´       `-----.___               ___ V 
//   |`.      _,-´  \     omega             `---.__        ,´| 
//   |  `.  ,´theta  \                              `--. ,´  | 
//        `.          \          H                     ,´ 
//          `.         \        /|\                  ,´ 
//            `.        \__.-----|-----.__         ,´ 
//              `._.---´ \ sigma | sigma   `---._,´ 
//                `.      \      |             ,´ 
//                  `.     \  _.-|           ,´ 
//                    `.    \´   |         ,´ 
//                      `.   \phi|       ,´ 
//                        `.  \  |     ,´ 
//                          `. \ |   ,´ 
//                            `.\| ,´ 
//                              `@´ 
//
vec3 Fresnel(float cos_theta, vec3 F0)
{
	return mix(vec3(pow(1.0 - cos_theta, 5.0)), vec3(1.0), F0); 
}

float NormalDistribution(float roughness, float cos_phi)
{
	// GGX
	float alpha       = roughness * roughness;
    float alpha_sqr   = alpha * alpha;
	float cos_phi_sqr = cos_phi*cos_phi;
	
	float nominator   = alpha_sqr;
    float denominator = (cos_phi_sqr * (alpha_sqr - 1.0) + 1.0);
    denominator = kPI * denominator * denominator;
	
    return nominator / denominator;	
}

float GeometrySchlickGGX(float alpha, float roughness) {
	float r = roughness + 1.0;
	float k = (r * r) / 8.0;

	float nominator = alpha;
	float denominator = alpha * (1.0 - k) + k;

	return nominator / denominator;
}

float GeometricShadowing(float cos_theta, float cos_omega, float roughness)
{
	//Smith
	float ggx1 = GeometrySchlickGGX(cos_omega, roughness);
	float ggx2 = GeometrySchlickGGX(cos_theta, roughness);
	return ggx1 * ggx2;
}

vec3 BRDF(vec3 n, vec3 l, vec3 v, Material mat)
{
    vec3 h = normalize(l + v);
		
	float cos_theta = dot(n, l);
	
	if(0 > cos_theta )
		return vec3(0);
		
	float cos_omega = max(dot(n,v), 0);
	float cos_phi   = max(dot(n,h), 0);
	float cos_sigma = max(dot(v,h), 0);
		
	vec3  F = Fresnel(cos_theta, mat.specular_color);
	float D = NormalDistribution(mat.roughness, cos_phi);
	float G = GeometricShadowing(cos_theta, cos_omega, mat.roughness);
	
	vec3 specular = (F*D*G)/(4*cos_theta*cos_omega + 0.001);
	vec3 diffuse  = (1-F)*mat.diffuse_color/kPI;
	return cos_theta*(specular + diffuse);	
}