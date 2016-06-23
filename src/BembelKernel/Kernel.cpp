/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "Kernel.h"
#include "Display/DisplayManager.h"
#include "Engine/Engine.h"

#include <BembelBase/Events/EventManager.h>
#include <BembelBase/Logging/Logger.h>
#include <BembelBase/XML.h>

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

std::shared_ptr<EventManager> Kernel::GetEventManager() const
{
	return _eventMgr;
}

std::shared_ptr<DisplayManager> Kernel::GetDisplayManager() const
{
	return _displayMgr;
}

Engine* Kernel::GetEngine() const
{
	return _engine.get();
}

void Kernel::PollEvents()
{
	glfwPollEvents();
}

bool Kernel::LoadSetting(const std::string& configFileName)
{
	xml::Document doc;
	if (doc.LoadFile(configFileName.c_str()) != tinyxml2::XML_SUCCESS)
	{
		BEMBEL_LOG_ERROR()
			<< "Failed to lode file '" << configFileName << "'\n"
			<< doc.ErrorName() << std::endl;
		return false;
	}

	const xml::Element* root = doc.FirstChildElement("Bembel");
	if (!root)
		return false;

	const xml::Element* display = root->FirstChildElement("Display");
	if (display)
		_displayMgr->CreateWindows(display);

	const xml::Element* systems = root->FirstChildElement("Systems");
	if (systems)
		_engine->InitSystems(systems);

	return true;
}

} //end of namespace JHL
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
