#version 330 compatibility

layout(location = 0) in vec4  aPositions;
layout(location = 1) in vec4  aTexCords;
layout(location = 2) in vec4  aColor;
layout(location = 3) in vec4  aData;

uniform vec2 uViewPortSize;

out vec2 vTexCoords;
out vec4 vColor;
out vec4 vData;

const vec2 gOffsets[4] = vec2[](
	vec2( 0.0, 0.0 ),
	vec2( 1.0, 0.0 ),
	vec2( 0.0, 1.0 ),
	vec2( 1.0, 1.0 )
);

void main()
{
	vec2 offset     = gOffsets[gl_VertexID];
	vec2 pos        = aPositions.xy + offset*aPositions.zw;
	gl_Position     = vec4(2*pos/uViewPortSize - vec2(1.0f), 0.0, 1.0 );
	vTexCoords      = aTexCords.xy + offset*aTexCords.zw;
	vColor          = aColor;
	vData           = aData;
}