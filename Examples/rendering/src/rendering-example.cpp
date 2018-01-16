/******************************************************************************/
/* ************************************************************************** */
/* *                                                                        * */
/* *    MIT License                                                         * */
/* *                                                                        * */
/* *   Copyright(c) 2018 Joachim Herber                                     * */
/* *                                                                        * */
/* *   Permission is hereby granted, free of charge, to any person          * */
/* *   obtaining copy of this software and associated documentation files   * */
/* *   (the "Software"), to deal in the Software without restriction,       * */
/* *   including without limitation the rights to use, copy, modify, merge, * */
/* *   publish, distribute, sublicense, and/or sell copies of the Software, * */
/* *   and to permit persons to whom the Software is furnished to do so,    * */
/* *   subject to the following conditions :                                * */
/* *                                                                        * */
/* *   The above copyright notice and this permission notice shall be       * */
/* *   included in all copies or substantial portions of the Software.      * */
/* *                                                                        * */
/* *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,      * */
/* *   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF   * */
/* *   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND                * */
/* *   NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS   * */
/* *   BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN   * */
/* *   ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN    * */
/* *   CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE     * */
/* *   SOFTWARE.                                                            * */
/* *                                                                        * */
/* ************************************************************************** */
/******************************************************************************/

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

