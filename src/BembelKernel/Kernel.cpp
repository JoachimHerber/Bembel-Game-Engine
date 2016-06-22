/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "Kernel.h"
#include "Display/DisplayManager.h"
#include "Engine/Engine.h"

#include <BembelBase/Events/EventManager.h>
#include <BembelBase/Logging/Logger.h>

#include <glfw/glfw3.h>

#include <chrono>

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel{

Kernel::Kernel()
	: _eventMgr(std::make_unique<EventManager>())
	, _engine(std::make_unique<Engine>())
{
	if( glfwInit() == GL_FALSE )
	{
		BEMBEL_LOG_ERROR() << "Failed to initialize GLFW" << std::endl;
		throw std::exception();
	}

	_displayMgr = std::make_unique<DisplayManager>(this);
}

Kernel::~Kernel()
{
	_displayMgr.reset();

	glfwTerminate();
}

EventManager* Kernel::GetEventManager() const
{
	return _eventMgr.get();
}

DisplayManager* Kernel::GetDisplayManager() const
{
	return _displayMgr.get();
}

Engine* Kernel::GetEngine() const
{
	return _engine.get();
}

void Kernel::PollEvents()
{
	glfwPollEvents();
}

} //end of namespace JHL
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
