#version 330

layout(location = 0) in vec3  aPosition;
layout(location = 2) in vec3  aColor;
layout(location = 1) in float aBulbRadius;
layout(location = 3) in float aCutoffRadius;

uniform mat4 uProjectionMatrix;

const vec3 gOffsets[10] = vec3[](
vec3(+0.0000,+0.0000,-1), 
vec3(-1.0000,+0.3742,+0), 
vec3(-0.3742,+1.0000,+0), 
vec3(+0.3742,+1.0000,+0), 
vec3(+1.0000,+0.3742,+0),
vec3(+1.0000,-0.3742,+0), 
vec3(+0.3742,-1.0000,+0),  
vec3(-0.3742,-1.0000,+0),  
vec3(-1.0000,-0.3742,+0), 
vec3(-1.0000,+0.3742,+0)
);

out vec3  vLightPos;
out vec3  vLightColor;
out float vBulbRadius;
out float vCutoffRadius;


void main()
{
	vLightPos     = aPosition;
	vLightColor   = aColor;
	vBulbRadius   = aBulbRadius;
	vCutoffRadius = aCutoffRadius;
	
	gl_Position = uProjectionMatrix*vec4(aPosition + aCutoffRadius*gOffsets[gl_VertexID], 1);
	gl_Position /= gl_Position.w;
	gl_Position.z = 0;
}