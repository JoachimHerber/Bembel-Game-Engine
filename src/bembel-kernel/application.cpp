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
