#version 330 compatibility

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;   
 
uniform mat4 uNormalMatrix;
uniform mat4 uModleViewMatrix;
uniform mat4 uProjectionMatrix;
 
out vec3 vNormal;
out vec4 vWorldPos;
out vec4 vRelPos;
 
void main()
{
 	vNormal   = normalize((uNormalMatrix*vec4(aNormal,0)).xyz);
 
    vWorldPos    = vec4(aPosition,1);
	vRelPos     = uModleViewMatrix*vWorldPos;
	vRelPos.xyz += 0.0005*vNormal;
 	gl_Position  = uProjectionMatrix*vRelPos;
}
