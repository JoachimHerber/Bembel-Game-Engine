/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include <BembelOpenGL.h>

#include "SimpleGeometryRenderer.h"
#include "SimpleGeometryComponent.h"

#include <BembelKernel/Scene/Scene.h>
#include <BembelKernel/Scene/PositionComponent.h>

#include <glm/gtc/matrix_transform.hpp>

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel{

const static std::string VERT = R"(
#version 330
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;  
layout(location = 2) in vec2 aTexCoord;  

uniform mat4 uNormalMatrix;
uniform mat4 uModleViewMatrix;
uniform mat4 uProjectionMatrix;

out vec3 vNormal;
out vec2 vTexCoord;

void main()
{
	vNormal   = normalize((uNormalMatrix*vec4(aNormal,0)).xyz);
	vTexCoord = aTexCoord;
	gl_Position = 
		uProjectionMatrix*uModleViewMatrix*vec4(aPosition,1);
}
)";
const static std::string FRAG = R"(
#version 330

in vec3 vNormal;
in vec2 vTexCoord;

uniform float     uRoughness;

uniform sampler2D uAlbedoTexture;

layout(location = 0) out vec4 oNormal;
layout(location = 1) out vec4 oAlbedo;

void main()
{
	oAlbedo       = texture(uAlbedoTexture, vTexCoord);
	oNormal.xyz   = vec3(0.5*normalize(vNormal) + vec3(0.5));
	oNormal.a     = oAlbedo.a;
}
)";

SimpleGeometryRenderer::SimpleGeometryRenderer()
{}

SimpleGeometryRenderer::~SimpleGeometryRenderer()
{}

void SimpleGeometryRenderer::Init()
{
	_shader = std::make_unique<bembel::ShaderProgram>();
	_shader->AttachShader(GL_VERTEX_SHADER, VERT);
	_shader->AttachShader(GL_FRAGMENT_SHADER, FRAG);
	if (!_shader->Link())
	{
		_shader.reset();
		return;
	}
	InitGeometry();
}

void SimpleGeometryRenderer::Cleanup()
{
	_shader.reset();
}

void SimpleGeometryRenderer::SetScene(ScenePtr scene)
{
	_scene = scene;

	_geometryContainer = 
		_scene->RequestComponentContainer<SimpleGeometryComponent>();
	_positionConteiner = 
		_scene->RequestComponentContainer<PositionComponent>();
	_textureConteiner = 
		_scene->GetAssetManager()->RequestAssetContainer<Texture>();
}

void SimpleGeometryRenderer::DoGeometryPass(
	const glm::mat4& view, 
	const glm::mat4& projection, 
	const ViewFrustum& frustum)
{
	_shader->Use();
	
	unsigned uNormalMatrix     = _shader->GetUniformLocation("uNormalMatrix");
	unsigned uModleViewMatrix  = _shader->GetUniformLocation("uModleViewMatrix");
	unsigned uProjectionMatrix = _shader->GetUniformLocation("uProjectionMatrix");
	unsigned uAlbedo    = _shader->GetUniformLocation("uAlbedo");
	unsigned uRoughness = _shader->GetUniformLocation("uRoughness");

	glUniformMatrix4fv(uProjectionMatrix, 1, GL_FALSE, &projection[0][0]);

 	glBindVertexArray(_vao);

	auto& entities  = _scene->GetEntitys();
	auto& positions = _positionConteiner->GetComponents();
	auto& geometry  = _geometryContainer->GetComponents();

	Scene::ComponentMask mask =
		_geometryContainer->GetComponentMask() |
		_positionConteiner->GetComponentMask();

	for (Scene::EntityID entity = 0; entity<entities.size(); ++entity)
	{
		if((entities[entity] & mask) != mask)
			continue;

		Texture* albedo = _textureConteiner->GetAsset(geometry[entity].albedo);
		if (!albedo)
			continue;

		albedo->Bind();

		glm::mat4 modleView;
		modleView = glm::translate(modleView, positions[entity].position);
		modleView = glm::scale(modleView, geometry[entity].size);
		modleView = view*modleView;
		glm::mat4 normalMatix = glm::transpose(glm::inverse(modleView));

		glUniformMatrix4fv(uNormalMatrix,  1, GL_FALSE, &normalMatix[0][0]);
		glUniformMatrix4fv(uModleViewMatrix, 1, GL_FALSE, &modleView[0][0]);
		glUniform1f(uRoughness, geometry[entity].roughness);

		auto& shape = _shapes[geometry[entity].shape];
		glDrawElements(GL_TRIANGLES, shape.second, GL_UNSIGNED_INT, (void*)(shape.first));
	}
 	glBindVertexArray(0);

	glUseProgram(0);
}

void SimpleGeometryRenderer::DoShadowPass(
	const glm::mat4& view, 
	const glm::mat4& projection, 
	const ViewFrustum& frustum)
{
	
}

std::unique_ptr<SimpleGeometryRenderer> SimpleGeometryRenderer::CreateInstance(const xml::Element*)
{
	return std::make_unique<SimpleGeometryRenderer>();
}

void SimpleGeometryRenderer::InitGeometry()
{
	std::vector<VertexData> vertices;
	std::vector<unsigned> indices;
	unsigned startIndex = vertices.size();
	_shapes.resize(SimpleGeometryComponent::NUM_SHAPES);

	InitSpereGeometry(vertices, indices);
	InitXZPlaneGeometry(vertices, indices);

	InitVBO(vertices);
	InitIBO(indices);
	InitVAO();
}

void SimpleGeometryRenderer::InitSpereGeometry(
	std::vector<VertexData>& vertices, 
	std::vector<unsigned>& indices)
{
	vertices.push_back({
		glm::vec3(0.0f, 0.5f, 0.0f), 
		glm::vec3(0.0f, 1.0f, 0.0f), 
		glm::vec2(0.5f, 1.0f)});

	const float pi = 3.14159265359;

	glm::vec3 offset;
	for (int i = 1; i < 16; ++i)
	{
		float v = 1 - i/16.0f;
		float f = pi*i/16.0f;
		float r = sin(f);
		offset.y = cos(f);
		for (int j = 0; j <= 32; ++j)
		{
			float u = j/32.0f;
			f = pi*j/16.0f;
			offset.x = r*sin(f);
			offset.z = r*cos(f);
			vertices.push_back({
				0.5f*offset, offset, glm::vec2(u,v)});
		}
	}
	vertices.push_back({
		glm::vec3(0.0f,-0.5f, 0.0f),
		glm::vec3(0.0f,-1.0f, 0.0f),
		glm::vec2(0.5f, 0.0f)});

	unsigned startIndex = 0;
	for (int j = 0; j < 32; ++j)
	{
		indices.push_back(startIndex + 0);
		indices.push_back(startIndex + j + 1 );
		indices.push_back(startIndex + j + 2 );
	}
	++startIndex;
	for (int i = 1; i < 15; ++i)
	{
		for (int j = 0; j <= 32; ++j)
		{
			indices.push_back(startIndex + ((j+0)));
			indices.push_back(startIndex + ((j+1)) + 33);
			indices.push_back(startIndex + ((j+1)));

			indices.push_back(startIndex + ((j+0)));
			indices.push_back(startIndex + ((j+0)) + 33);
			indices.push_back(startIndex + ((j+1)) + 33);
		}
		startIndex += 33;
	}
	for (int j = 0; j < 32; ++j)
	{
		indices.push_back(startIndex + j);
		indices.push_back(startIndex + 33);
		indices.push_back(startIndex + j + 1);
	}

	_shapes[SimpleGeometryComponent::SPHERE].first  = 0;
	_shapes[SimpleGeometryComponent::SPHERE].second = indices.size();
}

void SimpleGeometryRenderer::InitXZPlaneGeometry(
	std::vector<VertexData>& vertices,
	std::vector<unsigned>& indices)
{
	_shapes[SimpleGeometryComponent::XZ_PLAIN].first  = indices.size()*sizeof(unsigned);
	_shapes[SimpleGeometryComponent::XZ_PLAIN].second = 6;

	unsigned startIndex = vertices.size();
	vertices.push_back({glm::vec3(-0.5f, 0.0f, -0.5f),glm::vec3(0,1,0),glm::vec2(0,0)});
	vertices.push_back({glm::vec3(-0.5f, 0.0f, +0.5f),glm::vec3(0,1,0),glm::vec2(0,1)});
	vertices.push_back({glm::vec3(+0.5f, 0.0f, -0.5f),glm::vec3(0,1,0),glm::vec2(1,0)});
	vertices.push_back({glm::vec3(+0.5f, 0.0f, +0.5f),glm::vec3(0,1,0),glm::vec2(1,1)});

	indices.push_back(startIndex + 0);
	indices.push_back(startIndex + 1);
	indices.push_back(startIndex + 3);

	indices.push_back(startIndex + 0);
	indices.push_back(startIndex + 3);
	indices.push_back(startIndex + 2);
}

void SimpleGeometryRenderer::InitVBO(const std::vector<VertexData>& data)
{
	glGenBuffers(1, &_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, data.size()*sizeof(VertexData), &data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void SimpleGeometryRenderer::InitIBO(const std::vector<unsigned>& data)
{
	glGenBuffers(1, &_ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size()*sizeof(unsigned), &data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void SimpleGeometryRenderer::InitVAO()
{
	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(0*sizeof(float)));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3*sizeof(float)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(6*sizeof(float)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
	glBindVertexArray(0);
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/