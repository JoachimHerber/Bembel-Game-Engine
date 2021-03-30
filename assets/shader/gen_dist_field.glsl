<ShaderProgram>
  <Shader type="GL_VERTEX_SHADER">
#version 150 compatibility

out vec2  vTexCoord;

void main()
{
	vTexCoord = gl_MultiTexCoord0.xy;
	
	gl_Position = ftransform();
}
  </Shader>
  <Shader type="GL_FRAGMENT_SHADER">
#version 150

in vec2 vTexCoord;

out vec4 oFragColor;

void main()
{
	float r = clamp(1-length(vTexCoord.xy),0,1);
	oFragColor = vec4(r,0,0,r);
}
  </Shader>
</ShaderProgram>