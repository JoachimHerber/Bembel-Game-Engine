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
{
	noise_ = std::make_unique<bembel::Texture>(GL_TEXTURE_3D, GL_R8 );

	unsigned char data[32*32*32]; 
	std::random_device rd;
	std::mt19937 gen( rd() );
	std::uniform_int_distribution<> dist( 0, 255 );
	for( unsigned i = 0; i<32*32*32; ++i )
		data[i] = dist( gen );

	noise_->Init();
	noise_->Bind();

	glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, static_cast<GLint>(GL_REPEAT) );
	glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, static_cast<GLint>(GL_REPEAT) );
	glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, static_cast<GLint>(GL_REPEAT) );
	glTexImage3D(
		GL_TEXTURE_3D, 0, static_cast<GLint>(GL_R8),
		32, 32, 32, 0, GL_RED, GL_UNSIGNED_BYTE, data );

	noise_->Release();

	start_time_ = std::chrono::high_resolution_clock::now();
}

SelectionRenderingStage::~SelectionRenderingStage()
{}

void SelectionRenderingStage::SetShader(bembel::AssetHandle shader)
{
	shader_ = shader;
}

void SelectionRenderingStage::SetDepthOutputTexture(const std::string& texture)
{
	fbo_->SetDepthAttechment(pipline_->GetTexture(texture));
}
void SelectionRenderingStage::SetColorOutputTexture(const std::string& texture)
{
	fbo_->SetColorAttechment(0, pipline_->GetTexture(texture));
}
void SelectionRenderingStage::Init()
{
	fbo_->Init();
}

void SelectionRenderingStage::Cleanup()
{
	fbo_->CleanUp();
}

void SelectionRenderingStage::DoRendering()
{
	auto program = GetAssetManager()->GetAsset<bembel::ShaderProgram>(shader_);
	if (!program )
 		return;
 
 	fbo_->BeginRenderToTexture();
	glm::mat4 proj = pipline_->GetCamera()->GetProjectionMatrix();
	glm::mat4 view = pipline_->GetCamera()->GetViewMatrix();

 	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
 
	program->Use();
	noise_->Bind();
 
 	glUniformMatrix4fv(
		program->GetUniformLocation("uProjectionMatrix"),
 		1, GL_FALSE, &proj[0][0]);

	auto now = std::chrono::high_resolution_clock::now();
	std::chrono::milliseconds ms =
		std::chrono::duration_cast<std::chrono::milliseconds>(now - start_time_);

	float time = 0.001f*(ms.count());

	glUniform1f(program->GetUniformLocation( "uTime" ), time );

	std::vector<GeometryObject> geometry;

	GetHiglightedObjects(geometry);

	std::sort(geometry.begin(), geometry.end(), [ ](auto& first, auto& second){
		return first.dist > second.dist;
	});
 
	bembel::GeometryMesh* currentMesh = nullptr;
 	for (auto& it : geometry)
	{
		glm::mat4 modelView = view*glm::translate(glm::mat4(), it.position)*glm::mat4_cast(it.rotation);

		glUniform1i(program->GetUniformLocation("uState"), it.state);

		glUniformMatrix4fv(
			program->GetUniformLocation("uModleViewMatrix"),
			1, GL_FALSE, &modelView[0][0]);
		glUniformMatrix4fv(
			program->GetUniformLocation("uNormalMatrix"),
			1, GL_FALSE, &modelView[0][0]);

		auto mesh = scene_->GetAsset<bembel::GeometryMesh>(
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

	noise_->Release();
 	fbo_->EndRenderToTexture();
	glDisable(GL_BLEND);
}

void SelectionRenderingStage::SetScene(bembel::Scene* scene)
{
	scene_ = scene;
	position_components_  = scene->RequestComponentContainer<bembel::PositionComponent>();
	rotation_components_  = scene->RequestComponentContainer<bembel::RotationComponent>();
	selection_components_ = scene->RequestComponentContainer<SelectionComponent>();
	geometry_components_  = scene->RequestComponentContainer<bembel::GeometryComponent>();
}

std::unique_ptr<SelectionRenderingStage> SelectionRenderingStage::CreateInstance(
	const bembel::xml::Element* properties,
	bembel::RenderingPipeline* pipline)
{
	auto asset_manager = pipline->GetAssetManager();
	auto shader = asset_manager->RequestAsset<bembel::ShaderProgram>(
		properties->FirstChildElement("Shader"));

	auto stage = std::make_unique<SelectionRenderingStage>(pipline);

	stage->SetShader(shader);

	std::string textureName;
	if ( bembel::xml::GetAttribute(properties, "ColorOutput", "texture", textureName))
		stage->SetColorOutputTexture(textureName);
	if ( bembel::xml::GetAttribute(properties, "DepthOutput", "texture", textureName))
		stage->SetDepthOutputTexture(textureName);

	return std::move(stage);
}

void SelectionRenderingStage::GetHiglightedObjects(
	std::vector<GeometryObject>& objects)
{
	glm::vec3 camPos = pipline_->GetCamera()->GetPosition();

	const auto& entitis = scene_->GetEntitys();

	auto& positions = position_components_->GetComponents();
	auto& rotations = rotation_components_->GetComponents();
	auto& selection = selection_components_->GetComponents();
	auto& geometry  = geometry_components_->GetComponents();

	bembel::Scene::ComponentMask mask =
		position_components_->GetComponentMask() |
		selection_components_->GetComponentMask() |
		geometry_components_->GetComponentMask();

	for ( bembel::Scene::EntityID entity = 0; entity < entitis.size(); ++entity)
	{
		if ((entitis[entity] & mask)!= mask)
			continue;

		if (selection[entity].state == SelectionComponent::UNSELECTABLE)
			continue;

		auto model = scene_->GetAsset<bembel::GeometryModel>(
			geometry[entity].model);

		if(model == nullptr)
			continue;

		float dist = glm::length(camPos - positions[entity].position);

		glm::quat roatation;
		if( entitis[entity] & rotation_components_->GetComponentMask() )
			roatation = rotations[entity].rotation;

		objects.push_back({
			positions[entity].position, roatation,
			dist, model, unsigned(selection[entity].state - 1)
		});
	}
}
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
