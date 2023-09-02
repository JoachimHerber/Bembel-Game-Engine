#version 330 compatibility

in vec2 vTexCoords;
in vec4 vColor;
in vec4 vData;

uniform sampler2D uFontTexture;
uniform sampler2D uAtlasTexture;

layout(location = 0) out vec4 oColor;

void main()
{
    oColor = vColor;
	switch(uint(vData.a)){
		case 0u: {
		} break;
		case 1u: {
			float dist = texture2D(uFontTexture, vTexCoords).r;
			oColor.a *= smoothstep(vData.x/255, vData.y/255, dist);
		} break;
		case 2u: {
			vec4 mix = texture2D(uAtlasTexture, vTexCoords);
			oColor *= mix;
			oColor.rgb += (vec3(1)-mix.rgb) *  vData.rgb / 255.0f;
		} break;
	}
}
