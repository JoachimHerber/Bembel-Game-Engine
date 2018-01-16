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

#include "application.h"
#include "kernel.h"
#include "events/event-manager.h"
#include "display/display-manager.h"

#include <chrono>

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel {

Application::Application()
	: kernel_(std::make_unique<Kernel>())
{}

Application::~Application()
{}

bool Application::Run()
{
	if( !Init() )
		return false;

	MainLoop();

	Cleanup();

	return true;
}

void Application::Quit()
{
	quite_ = true;
}

void Application::MainLoop()
{
	auto time = std::chrono::high_resolution_clock::now();

	quite_ = false;
	while( !quite_ )
	{
		auto now = std::chrono::high_resolution_clock::now();
		std::chrono::milliseconds ms =
			std::chrono::duration_cast<std::chrono::milliseconds>(now - time);

		double timeSinceLastUpdate = 0.001*(ms.count());

		kernel_->PollEvents();
		Update(timeSinceLastUpdate);
		kernel_->UpdateSystems(timeSinceLastUpdate);
		kernel_->GetDisplayManager()->UpdateWindows();

		time = now;
	}
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
