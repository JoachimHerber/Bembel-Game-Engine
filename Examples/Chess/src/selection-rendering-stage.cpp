/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "selection-rendering-stage.h"

#include <bembel-kernel/rendering/texture.h>
#include <bembel-kernel/rendering/shader.h>
#include <bembel-kernel/rendering/frame-buffer-object.h>
#include <bembel-graphics/rendering-pipeline/rendering-pipeline.h>
#include <bembel-graphics/rendering-pipeline/camera.h>
#include <bembel-graphics/geometry/geometry-model.h>
#include <bembel-graphics/geometry/geometry-mesh.h>

#include "selection-component.h"

#include <glm/gtc/matrix_transform.hpp>

#include <random>

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
SelectionRenderingStage::SelectionRenderingStage(
	bembel::RenderingPipeline* pipline)
	: RenderingStage(pipline)
	, _fbo(std::make_unique<bembel::FrameBufferObject>())
{
	_noise = std::make_shared<bembel::Texture>(GL_TEXTURE_3D, GL_R8 );

	unsigned char data[32*32*32]; 
	std::random_device rd;
	std::mt19937 gen( rd() );
	std::uniform_int_distribution<> dist( 0, 255 );
	for( unsigned i = 0; i<32*32*32; ++i )
		data[i] = dist( gen );

	_noise->Init();
	_noise->Bind();

	glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, static_cast<GLint>(GL_REPEAT) );
	glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, static_cast<GLint>(GL_REPEAT) );
	glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, static_cast<GLint>(GL_REPEAT) );
	glTexImage3D(
		GL_TEXTURE_3D, 0, static_cast<GLint>(GL_R8),
		32, 32, 32, 0, GL_RED, GL_UNSIGNED_BYTE, data );

	_noise->Release();

	_starTime = std::chrono::high_resolution_clock::now();
}

SelectionRenderingStage::~SelectionRenderingStage()
{}

void SelectionRenderingStage::SetShader(ShaderPtr shader)
{
	_shader = shader;
}

void SelectionRenderingStage::SetDepthOutputTexture(const std::string& texture)
{
	_fbo->SetDepthAttechment(pipline_->GetTexture(texture));
}
void SelectionRenderingStage::SetColorOutputTexture(const std::string& texture)
{
	_fbo->SetColorAttechment(0, pipline_->GetTexture(texture));
}
void SelectionRenderingStage::Init()
{
	_fbo->Init();
}

void SelectionRenderingStage::Cleanup()
{
	_fbo->CleanUp();
}

void SelectionRenderingStage::DoRendering()
{
	if (!_shader)
 		return;
 
 	_fbo->BeginRenderToTexture();
	glm::mat4 proj = pipline_->GetCamera()->GetProjectionMatrix();
	glm::mat4 view = pipline_->GetCamera()->GetViewMatrix();

 	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
 
 	_shader->Use(); 
	_noise->Bind();
 
 	glUniformMatrix4fv(
 		_shader->GetUniformLocation("uProjectionMatrix"),
 		1, GL_FALSE, &proj[0][0]);

	auto now = std::chrono::high_resolution_clock::now();
	std::chrono::milliseconds ms =
		std::chrono::duration_cast<std::chrono::milliseconds>(now - _starTime);

	float time = 0.001f*(ms.count());

	glUniform1f( _shader->GetUniformLocation( "uTime" ), time );

	std::vector<GeometryObject> geometry;

	GetHiglightedObjects(geometry);

	std::sort(geometry.begin(), geometry.end(), [ ](auto& first, auto& second){
		return first.dist > second.dist;
	});
 
	bembel::GeometryMesh* currentMesh = nullptr;
 	for (auto& it : geometry)
	{
		glm::mat4 modelView = view*glm::translate(glm::mat4(), it.position);

		glUniform1i(_shader->GetUniformLocation("uState"), it.state);

		glUniformMatrix4fv(
			_shader->GetUniformLocation("uModleViewMatrix"),
			1, GL_FALSE, &modelView[0][0]);
		glUniformMatrix4fv(
			_shader->GetUniformLocation("uNormalMatrix"),
			1, GL_FALSE, &modelView[0][0]);

		auto mesh = _scene->GetAsset<bembel::GeometryMesh>(
			it.model->GetMesh());

		if(mesh == nullptr)
			continue;

		if (mesh != currentMesh)
		{
			currentMesh = mesh;
			glBindVertexArray(currentMesh->GetVAO());
		}

		for (auto& mapping : it.model->GetMateialMapping())
		{
			unsigned first, count;
			if (currentMesh->GetSubMesh(mapping.sub_mesh, first, count))
			{
				glDrawElements(
					GL_TRIANGLES,
					count,
					GL_UNSIGNED_INT,
					(void*)(first * sizeof(unsigned))
				);
			}
		}

 	}

	_noise->Release();
 	_fbo->EndRenderToTexture();
	glDisable(GL_BLEND);
}

void SelectionRenderingStage::SetScene(ScenePtr scene)
{
	_scene = scene;
	_positionComponents  = scene->RequestComponentContainer<bembel::PositionComponent>();
	_selectionComponents = scene->RequestComponentContainer<SelectionComponent>();
	_geometryComponents  = scene->RequestComponentContainer<bembel::GeometryComponent>();
}

std::unique_ptr<SelectionRenderingStage> SelectionRenderingStage::CreateInstance(
	const bembel::xml::Element* properties,
	bembel::RenderingPipeline* pipline)
{
	auto shader = CreateShader(properties->FirstChildElement("Shader"));

	auto stage = std::make_unique<SelectionRenderingStage>(pipline);

	stage->SetShader(shader);

	std::string textureName;
	if ( bembel::xml::GetAttribute(properties, "ColorOutput", "texture", textureName))
		stage->SetColorOutputTexture(textureName);
	if ( bembel::xml::GetAttribute(properties, "DepthOutput", "texture", textureName))
		stage->SetDepthOutputTexture(textureName);

	return std::move(stage);
}

std::shared_ptr<bembel::Shader> SelectionRenderingStage::CreateShader(
	const bembel::xml::Element* properties)
{
	if (!properties)
		return nullptr;

	std::string filename;

	auto program = std::make_shared<bembel::Shader>();
	for (auto shader : bembel::xml::IterateChildElements(properties, "VertexShader"))
	{
		if ( bembel::xml::GetAttribute(shader, "file", filename))
			program->AttachShaderFromFile(GL_VERTEX_SHADER, filename);
	}
	for (auto shader : bembel::xml::IterateChildElements(properties, "FragmentShader"))
	{
		if ( bembel::xml::GetAttribute(shader, "file", filename))
			program->AttachShaderFromFile(GL_FRAGMENT_SHADER, filename);
	}

	if (!program->Link())
		return nullptr;

	return program;
}

void SelectionRenderingStage::GetHiglightedObjects(
	std::vector<GeometryObject>& objects)
{
	glm::vec3 camPos = pipline_->GetCamera()->GetPosition();

	const auto& entitis = _scene->GetEntitys();

	auto& positions = _positionComponents->GetComponents();
	auto& selection = _selectionComponents->GetComponents();
	auto& geometry  = _geometryComponents->GetComponents();

	bembel::Scene::ComponentMask mask =
		_positionComponents->GetComponentMask() |
		_selectionComponents->GetComponentMask() |
		_geometryComponents->GetComponentMask();

	for ( bembel::Scene::EntityID entity = 0; entity < entitis.size(); ++entity)
	{
		if ((entitis[entity] & mask)!= mask)
			continue;

		if (selection[entity].state == SelectionComponent::UNSELECTABLE)
			continue;

		auto model = _scene->GetAsset<bembel::GeometryModel>(
			geometry[entity].model);

		if(model == nullptr)
			continue;

		float dist = glm::length(camPos - positions[entity].position);

		objects.push_back({
			positions[entity].position,
			dist, model, unsigned(selection[entity].state - 1)
		});
	}
}
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
