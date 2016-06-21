/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "Application.h"
#include "Engine.h"

#include <BembelBase/Events/EventManager.h>

#include <chrono>

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel{

Application::Application()
	: _eventMgr(std::make_shared<EventManager>())
	, _engine(std::make_unique<Engine>())
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

		Update(0.001*(ms.count()));
		_engine->UpdateSystems(0.001*(ms.count()));

		time = now;
	}
}

} //end of namespace JHL
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
