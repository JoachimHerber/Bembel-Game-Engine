#version 330

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;  
layout(location = 2) in vec2 aTexCoord;  
 
uniform mat4 uNormalMatrix;
uniform mat4 uModleViewMatrix;
uniform mat4 uProjectionMatrix;
 
out vec3 vNormal;
out vec2 vTexCoord;
 
 
void main()
{
 	vNormal   = normalize((uNormalMatrix*vec4(aNormal,0)).xyz);
	vTexCoord = aTexCoord;
 
 	gl_Position = 
 		uProjectionMatrix*uModleViewMatrix*vec4(aPosition,1);
}