#version 330
 
uniform mat4 uInverseProjectionMatrix;
uniform mat4 uInverseViewMatrix;

uniform samplerCube uEnvironmentMap;

uniform sampler2D uDepthBuffer;
uniform sampler2D uAlbedoBuffer;
uniform sampler2D uReflectivityBuffer;
uniform sampler2D uNormalBuffer;

in vec2 vTexCoord;

const float PI = 3.14159265359;

layout(location = 0) out vec3 oColor;

vec3 DecodeNormal(vec3 n)
{
    return normalize(2*n - vec3(1));
}
// 
//             N 
//             /\`-._ 
//            /  \ 
//                \        _____ 
//  L ___         _\.----´       `-----.___               ___ V 
//   |`.      _,-´  \  theta2               `---.__        ,´| 
//   |  `.  ,´theta1 \                              `--. ,´  | 
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

float GGX_Normal_Distribution(float alpha)
{
	float f = (alpha*alpha -1) + 1;
	return alpha*alpha/(PI*f*f); 
} 

float Geometric_Shadowing_Schlick(float k, float cosTheta) 
{	
	return 1 / ( (cosTheta*(1-k)+k) * (cosTheta*(1-k)+k) ); 
}

vec3 CookTorranceBRDF(vec3 n, vec3 v, vec3 l, vec3 diffuseColor, vec3 F0, float roughness) 
{ 	
	roughness = max(roughness, 0.0000000000001);
	vec3 h = normalize(v + l);
	float cosTheta = dot(n, l); 
	// F is the fresnel them it describs the amount of secular reflected ligth depending on the viewing angle. 	
	// Light which is not secular reflected is either absorbt (metals) or diffusely reflected (non metals). 	
	vec3  F = F0 + (1 - F0)*pow(1-cosTheta, 5);
	// D is the the pobabilety densety for the orientation of microfacets. 	
	float D = GGX_Normal_Distribution(roughness*roughness); 	
	// G is the geometrc shodowing therm and describe the attenuation of the light due to the microfacets shadowing each other.  	
	float G = Geometric_Shadowing_Schlick(roughness/2, cosTheta); 		
	// 
	//              F*D*G 	
	// secular = ----------- 	
	//           4(l*n)(v*n) 
	// 	
	// the 1/((l*n)(v*n)) therm cancels the (l*n)(v*n) therm from the Geometric_Shadowing 	
	vec3 secular = max(F*D*G/(4), vec3(0)); 
	vec3 diffuse = max((1-F)*diffuseColor, vec3(0));  	
	return secular + diffuse;
}

void main()
{
	float deapt = texelFetch( uDepthBuffer, ivec2(gl_FragCoord.xy), 0 ).r;
	vec3 albedo = texelFetch( uAlbedoBuffer, ivec2(gl_FragCoord.xy), 0 ).rgb;
	vec3 F0    = texelFetch( uReflectivityBuffer, ivec2(gl_FragCoord.xy), 0 ).rgb;
	vec4 n     = texelFetch( uNormalBuffer, ivec2(gl_FragCoord.xy), 0 );
	
	vec4 position = uInverseProjectionMatrix*vec4( 2*vTexCoord - vec2(1.0), 2*deapt - 1, 1 );
	position.xyz /= position.w;
	
	oColor = vec3(0);
	if(deapt==1)
	{
		vec3 viewDir = (uInverseViewMatrix*vec4(normalize(position.xyz),0)).xyz;
		oColor = texture(uEnvironmentMap, viewDir).rgb;
	}
	else
	{
		vec3 normal = DecodeNormal(n.xyz);
		float roughness = n.a;
		vec3 v = normalize(-position.xyz);
		vec3 r = normalize(-reflect(v, normal));
		
		float cosTheta = dot( v, normal ); 		
		if(cosTheta > 0)
			oColor = cosTheta*CookTorranceBRDF(normal, v, r, albedo, F0, roughness)*textureLod(uEnvironmentMap, (uInverseViewMatrix*vec4(r,0)).xyz, 8*roughness).rgb;
	}
}