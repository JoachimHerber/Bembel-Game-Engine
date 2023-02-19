#version 330

const vec2 g_vectices[4]  =  vec2[](vec2(-1,-1), vec2(+1,-1), vec2(-1,+1), vec2(+1,+1));
const vec2 g_texCoords[4] = vec2[](vec2(0,0), vec2(1,0), vec2(0,1), vec2(1,1));

out vec2 vTexCoord;

void main()
{
	gl_Position = vec4( g_vectices[gl_VertexID], 0, 1);
	vTexCoord    = g_texCoords[gl_VertexID];
}
