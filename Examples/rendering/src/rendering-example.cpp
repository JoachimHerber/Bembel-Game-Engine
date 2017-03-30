/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/
#include "rendering-example.h"

#include <bembel-kernel/kernel.h>
#include <bembel-kernel/display/display-manager.h>
#include <bembel-graphics/rendering-pipeline/rendering-pipeline.h>

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
RenderingExample::RenderingExample()
	: bembel::Application()
{
	graphic_system_ = kernel_->AddSystem<bembel::GraphicSystem>();

	kernel_->GetEventManager()->AddHandler<bembel::WindowShouldCloseEvent>(this);
}

RenderingExample::~RenderingExample()
{}

bool RenderingExample::Init()
{
	if (!kernel_->LoadSetting("rendering/config.xml"))
		return false;
	
	cam_ = std::make_unique<CameraControle>(
		kernel_->GetEventManager(), 
		graphic_system_->GetRenderingPiplies()[0]->GetCamera()
	);

	scenes_.push_back( std::make_shared<bembel::Scene>(kernel_->GetAssetManager()));

	graphic_system_->GetRenderingPiplies()[0]->SetScene(scenes_[0]);

	scenes_[0]->LoadScene("scenes/material-test.scene");

	kernel_->InitSystems();
	return true;
}

void RenderingExample::Cleanup()
{
	kernel_->ShutdownSystems();
	kernel_->GetDisplayManager()->CloseOpenWindows();
}

void RenderingExample::Update(double time)
{
	cam_->Update(time);
}

void RenderingExample::HandleEvent(const bembel::WindowShouldCloseEvent& event)
{
	Quit();
}

/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/

