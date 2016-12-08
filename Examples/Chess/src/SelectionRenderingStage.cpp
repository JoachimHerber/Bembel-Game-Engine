/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "SelectionRenderingStage.h"
#include <BembelGraphics/RenderingPipeline/RenderingPipeline.h>
#include <BembelGraphics/RenderingPipeline/Camera.h>

#include <BembelKernel/Rendering/Texture.h>
#include <BembelKernel/Rendering/Shader.h>
#include <BembelKernel/Rendering/FrameBufferObject.h>

#include "SelectionComponent.h"

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
 
 	glEnable(GL_DEPTH_TEST);
 	glDisable(GL_CULL_FACE);
 	glDisable(GL_BLEND);
 
 	_shader->Use(); 
 
 	glm::mat4 proj = _pipline->GetCamera()->GetProjectionMatrix();
 	glm::mat4 view = _pipline->GetCamera()->GetViewMatrix();
 	glm::mat4 mvp = proj*view;
 
 	glUniformMatrix4fv(
 		_shader->GetUniformLocation("uModleViewProjectionMatrix"),
 		1, GL_FALSE, &mvp[0][0]);
 
 	const auto& entitis = _scene->GetEntitys();
 
 	Scene::ComponentMask mask = 
 		_positionComponents->GetComponentMask() |
 		_selectionComponents->GetComponentMask();
 
 	glLineWidth(4);
 	glBegin(GL_LINES);
 	for (Scene::EntityID entity = 0; entity < entitis.size(); ++entity)
 	{
 		if((entitis[entity] & mask)!= mask)
 			continue;
 
 		auto* selectionComp = _selectionComponents->GetComponent(entity);
 
 		glm::vec3 pos = _positionComponents->GetComponent(entity)->position;
 
		switch (selectionComp->stat)
		{
		case SelectionComponent::SELECTABLE:
			glColor3f(1.0f, 1.0f, 1.0f);
			break;
		case SelectionComponent::FOCUSED:
			glColor3f(0.0f, 1.0f, 1.0f);
			break;
		case SelectionComponent::SELECED:
			glColor3f(1.0f, 1.0f, 0.0f);
			break;
		default:
			continue;
		}
		
		glVertex3f(pos.x + 1.0f, pos.y + 0.1f, pos.z + 1.0f);
		glVertex3f(pos.x + 1.0f, pos.y + 0.1f, pos.z - 1.0f);
		glVertex3f(pos.x + 1.0f, pos.y + 0.1f, pos.z - 1.0f);
		glVertex3f(pos.x - 1.0f, pos.y + 0.1f, pos.z - 1.0f);
		glVertex3f(pos.x - 1.0f, pos.y + 0.1f, pos.z - 1.0f);
		glVertex3f(pos.x - 1.0f, pos.y + 0.1f, pos.z + 1.0f);
		glVertex3f(pos.x - 1.0f, pos.y + 0.1f, pos.z + 1.0f);
		glVertex3f(pos.x + 1.0f, pos.y + 0.1f, pos.z + 1.0f);

 	}
 	glEnd(); 
 	_fbo->EndRenderToTexture();
}

void SelectionRenderingStage::SetScene(ScenePtr scene)
{
	_scene = scene;
	_positionComponents = scene->RequestComponentContainer<PositionComponent>();
	_selectionComponents = scene->RequestComponentContainer<SelectionComponent>();
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

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
