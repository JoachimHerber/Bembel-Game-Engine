<Shader type='vert'>
#version 150 compatibility

out vec2  vTexCoord;

void main()
{
	vTexCoord = gl_MultiTexCoord0.xy;
	
	gl_Position = ftransform();
}
</Shader>