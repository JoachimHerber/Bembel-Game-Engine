#version 330

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;  
layout(location = 2) in vec2 aTexCoord;  
 
uniform mat4 uNormalMatrix;
uniform mat4 uModleViewMatrix;
uniform mat4 uProjectionMatrix;
 
out DATA {
	vec3 normal;
	vec2 tex_coord;
} vert_data;
 
void main()
{
 	vert_data.normal    = normalize((uNormalMatrix*vec4(aNormal,0)).xyz);
	vert_data.tex_coord = aTexCoord;
 
 	gl_Position = 
 		uProjectionMatrix*uModleViewMatrix*vec4(aPosition,1);
		
}
