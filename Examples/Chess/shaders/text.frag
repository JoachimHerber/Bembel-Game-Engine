#version 330

uniform sampler2D uAtlasTexture;

in vec2 vTexCoords;

layout(location = 0) out vec4 oColor;
 

void main()
{	
	oColor = texture(uAtlasTexture, vTexCoords);
	oColor.a = 0.0;
}