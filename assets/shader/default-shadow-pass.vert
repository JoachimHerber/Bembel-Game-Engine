#version 330

layout(location = 0) in vec3 aPosition; 
 
uniform mat4 uModleViewMatrix;
uniform mat4 uProjectionMatrix;
 
void main()
{
 	gl_Position = uProjectionMatrix*uModleViewMatrix*vec4(aPosition, 1);
}
