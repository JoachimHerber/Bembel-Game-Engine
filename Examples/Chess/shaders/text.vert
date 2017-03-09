#version 330 compatibility  
 
uniform vec4 uResulution;
 
const vec2 gOffsets[4] = vec2[](
vec2( 0.0000, 1.0000 ), 
vec2( 0.0000, 0.0000 ), 
vec2( 1.0000, 1.0000 ), 
vec2( 1.0000, 0.0000 )
);

out vec2 vTexCoords;
 
void main()
{
	vTexCoords = gOffsets[gl_VertexID];

	gl_Position.xy = 0.5*uResulution.xy + 64*gOffsets[gl_VertexID];
	
	gl_Position.xy *= 2*uResulution.zw;
	
	gl_Position.xy = 2*vTexCoords;
	gl_Position.xy -= vec2(1, 1);
	gl_Position.z = 1;
	gl_Position.w = 1;
}