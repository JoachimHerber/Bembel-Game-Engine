#version 330 compatibility

layout(location = 0) in vec4 aPosition;
layout(location = 1) in vec4 aTexCoors;   
layout(location = 2) in vec4 aColor;   
 
uniform mat4 uViewMatrix;
uniform mat4 uProjectionMatrix;
 
out vec4 vColor;
out vec2 vTexCoors;
 
const vec2 gOffsets[4] = vec2[](
	vec2( 0.0, 0.0 ),
	vec2( 1.0, 0.0 ),
	vec2( 0.0, 1.0 ),
	vec2( 1.0, 1.0 )
);

void main()
{
	vec4 pos = uViewMatrix*vec4(aPosition.xyz, 1.0f);
	
	pos.xy += aPosition.w * gOffsets[gl_VertexID];
	
	vColor       = aColor;
	vTexCoors    = mix(aTexCoors.xy, aTexCoors.zw, gOffsets[gl_VertexID]);
 	gl_Position  = uProjectionMatrix*pos;
}
