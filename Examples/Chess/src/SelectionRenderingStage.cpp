/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "SelectionRenderingStage.h"
#include <BembelGraphics/RenderingPipeline/RenderingPipeline.h>
#include <BembelGraphics/RenderingPipeline/Camera.h>

#include <BembelKernel/Rendering/Texture.h>
#include <BembelKernel/Rendering/Shader.h>
#include <BembelKernel/Rendering/FrameBufferObject.h>
#include <BembelKernel/Rendering/Geometry/GeometryModel.h>
#include <BembelKernel/Rendering/Geometry/GeometryMesh.h>

#include "SelectionComponent.h"

#include <glm/gtc/matrix_transform.hpp>

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel {

SelectionRenderingStage::SelectionRenderingStage(
	RenderingPipeline* pipline)
	: RenderingStage(pipline)
	, _fbo(std::make_unique<FrameBufferObject>())
{}

SelectionRenderingStage::~SelectionRenderingStage()
{}

void SelectionRenderingStage::SetShader(ShaderPtr shader)
{
	_shader = shader;
}

void SelectionRenderingStage::SetDepthOutputTexture(const std::string& texture)
{
	_fbo->SetDepthAttechment(_pipline->GetTexture(texture));
}
void SelectionRenderingStage::SetColorOutputTexture(const std::string& texture)
{
	_fbo->SetColorAttechment(0, _pipline->GetTexture(texture));
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
	glm::mat4 proj = _pipline->GetCamera()->GetProjectionMatrix();
	glm::mat4 view = _pipline->GetCamera()->GetViewMatrix();


 	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
 
 	_shader->Use(); 
 
 
 	glUniformMatrix4fv(
 		_shader->GetUniformLocation("uProjectionMatrix"),
 		1, GL_FALSE, &proj[0][0]);
 
	std::vector<GeometryObject> geometry;

	GetHiglightedObjects(geometry);

	std::sort(geometry.begin(), geometry.end(), [ ](auto& first, auto& second){
		return first.dist > second.dist;
	});
 
	GeometryMesh* currentMesh = nullptr;
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

		auto mesh = _scene->GetAssetManager()->GetAsset<GeometryMesh>(
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
			if (currentMesh->GetSubMesh(mapping.subMesh, first, count))
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
 	_fbo->EndRenderToTexture();
	glDisable(GL_BLEND);
}

void SelectionRenderingStage::SetScene(ScenePtr scene)
{
	_scene = scene;
	_positionComponents  = scene->RequestComponentContainer<PositionComponent>();
	_selectionComponents = scene->RequestComponentContainer<SelectionComponent>();
	_geometryComponents  = scene->RequestComponentContainer<GeometryComponent>();
}

std::unique_ptr<SelectionRenderingStage> SelectionRenderingStage::CreateInstance(
	const xml::Element* properties,
	RenderingPipeline* pipline)
{
	auto shader = CreateShader(properties->FirstChildElement("Shader"));

	auto stage = std::make_unique<SelectionRenderingStage>(pipline);

	stage->SetShader(shader);

	std::string textureName;
	if (xml::GetAttribute(properties, "ColorOutput", "texture", textureName))
		stage->SetColorOutputTexture(textureName);
	if (xml::GetAttribute(properties, "DepthOutput", "texture", textureName))
		stage->SetDepthOutputTexture(textureName);

	return std::move(stage);
}

std::shared_ptr<Shader> SelectionRenderingStage::CreateShader(const xml::Element* properties)
{
	if (!properties)
		return nullptr;

	std::string filename;

	auto program = std::make_shared<Shader>();
	for (auto shader : xml::IterateChildElements(properties, "VertexShader"))
	{
		if (xml::GetAttribute(shader, "file", filename))
			program->AttachShaderFromFile(GL_VERTEX_SHADER, filename);
	}
	for (auto shader : xml::IterateChildElements(properties, "FragmentShader"))
	{
		if (xml::GetAttribute(shader, "file", filename))
			program->AttachShaderFromFile(GL_FRAGMENT_SHADER, filename);
	}

	if (!program->Link())
		return nullptr;

	return program;
}

void SelectionRenderingStage::GetHiglightedObjects(
	std::vector<GeometryObject>& objects)
{
	glm::vec3 camPos = _pipline->GetCamera()->GetPosition();

	const auto& entitis = _scene->GetEntitys();

	auto& positions = _positionComponents->GetComponents();
	auto& selection = _selectionComponents->GetComponents();
	auto& geometry  = _geometryComponents->GetComponents();

	Scene::ComponentMask mask =
		_positionComponents->GetComponentMask() |
		_selectionComponents->GetComponentMask() |
		_geometryComponents->GetComponentMask();

	for (Scene::EntityID entity = 0; entity < entitis.size(); ++entity)
	{
		if ((entitis[entity] & mask)!= mask)
			continue;

		if (selection[entity].state == SelectionComponent::UNSELECTABLE)
			continue;

		auto model = _scene->GetAssetManager()->GetAsset<GeometryModel>(
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

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
