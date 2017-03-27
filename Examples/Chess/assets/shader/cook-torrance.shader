<Shader type="GL_FRAGMENT_SHADER">
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
	//
	return clamp(F0 + (vec3(1)-F0)*pow(1-cos_theta, 5),0,1); 
}

float NormalDistribution(float alpha, float cos_phi)
{
	// GGX
	float alpha_squared = alpha*alpha;
	float cos_phi_squared = cos_phi*cos_phi;
	float den = cos_phi_squared*(alpha_squared -1) + 1;
	return cos_phi > 0 ? (alpha_squared/(kPI*den*den)) : 0; 
}

float GeometricShadowing(float cos_theta, float cos_omega, float cos_phi, float cos_sigma)
{
	//Cook−Torrance
	float f1 = (2*cos_phi*cos_theta)/(cos_sigma);
	float f2 = (2*cos_phi*cos_omega)/(cos_sigma);
    return min(1, min(f1, f2));
}

vec3 BRDF(vec3 n, vec3 l, vec3 v, Material mat)
{
    vec3 h = normalize(l + v);
		
	float cos_theta = dot(n,l);
	float cos_omega = dot(n,v);
	
	if(0 > cos_theta )
		return vec3(0);
		
	float cos_phi   = dot(n,h);
	float cos_sigma = dot(v,h);
		
	vec3  F = Fresnel(cos_theta, mat.specular_color);
	float D = NormalDistribution(mat.roughness*mat.roughness, cos_phi);
	float G = GeometricShadowing(cos_theta, cos_omega, cos_phi, cos_omega);
	
	vec3 specular = (F*D*G)/(4*cos_theta*cos_omega);
	vec3 diffuse  = (1-F)*mat.diffuse_color/(2*kPI);
	return cos_theta*(specular + diffuse);	
}
</Shader>