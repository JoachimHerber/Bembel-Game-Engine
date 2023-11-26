#version 330

in vec4 vColor;
in vec2 vTexCoors;

uniform sampler2D uTexture;

layout(location = 0) out vec4 oColor;
 
void main()
{	
 	oColor = vColor * texture2D(uTexture, vTexCoors);
	
	//vec2 v = 2.0f*vTexCoors - vec2(1.0f);
	//oColor.a *= smoothstep(0.0f, 1.0f, 1.0f-dot(v,v));
}

