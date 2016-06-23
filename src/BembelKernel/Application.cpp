/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "Application.h"
#include "Kernel.h"
#include "Events/EventManager.h"
#include "Display/DisplayManager.h"


#include <chrono>

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel{

Application::Application()
	: _kernel(std::make_unique<Kernel>())
{
}

Application::~Application()
{}

bool Application::Run()
{
	if (!Init())
		return false;

	MainLoop();

	Cleanup();

	return true;
}

void Application::Quit()
{
	_quite = true;
}

void Application::MainLoop()
{
	auto time = std::chrono::high_resolution_clock::now();

	_quite = false;
	while(!_quite)
	{
		auto now = std::chrono::high_resolution_clock::now();
		std::chrono::milliseconds ms =
			std::chrono::duration_cast<std::chrono::milliseconds>(now - time);

		double timeSinceLastUpdate = 0.001*(ms.count());

		_kernel->PollEvents();
		Update(timeSinceLastUpdate);
		_kernel->UpdateSystems(timeSinceLastUpdate);
		_kernel->GetDisplayManager()->UpdateWindows();

		time = now;
	}
}

} //end of namespace JHL
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
