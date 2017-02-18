/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "Kernel.h"
#include "System.h"
#include "Events/EventManager.h"
#include "Assets/AssetManager.h"
#include "Display/DisplayManager.h"

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
	, _assetMgr(std::make_unique<AssetManager>())
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

AssetManager* Kernel::GetAssetManager() const
{
	return _assetMgr.get();
}

DisplayManager* Kernel::GetDisplayManager() const
{
	return _displayMgr.get();
}

bool Kernel::AddSystem(std::shared_ptr<System> system)
{
	const std::string& name = system->GetName();

	auto it = _systemMapping.find(name);
	if (it != _systemMapping.end())
	{
		BEMBEL_LOG_ERROR()
			<< "System \""
			<< system->GetName()
			<< "\" has already been added."
			<< std::endl;
		return false;
	}

	for (size_t n = 0; n <_systems.size(); ++n)
	{
		if (!_systems[n])
		{
			_systems[n] = system;
			_systemMapping.emplace(name, n);
			return true;
		}
	}

	_systemMapping.emplace(name, _systems.size());
	_systems.push_back(system);
	return true;
}

bool Kernel::RemoveSystem(const std::string& name)
{
	auto it = _systemMapping.find(name);
	if (it == _systemMapping.end())
	{
		BEMBEL_LOG_WARNING() << "Unknown system '" << name << "'." << std::endl;
		return false;
	}

	_systems[it->second].reset();
	_systemMapping.erase(it);
	return true;
}

std::shared_ptr<System> Kernel::GetSystem(const std::string& name)
{
	auto it = _systemMapping.find(name);
	if (it != _systemMapping.end())
		return _systems[it->second];

	BEMBEL_LOG_WARNING() << "Unknown system '" << name << "'." << std::endl;
	return nullptr;
}

bool Kernel::InitSystems()
{
	for (auto system : _systems)
	{
		if (!system->Init())
			return false;
	}
	return true;
}

void Kernel::UpdateSystems(double timeSinceLastUpdate)
{
	for (auto system : _systems)
		system->Update(timeSinceLastUpdate);
}
void Kernel::ShutdownSystems()
{
	for (auto system : _systems)
		system->Shutdown();
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
	{
		for (auto system : _systems)
		{
			const xml::Element* systemConfig =
				systems->FirstChildElement(system->GetName().c_str());

			if (!system->Configure(systemConfig))
				return false;
		}
	}

	return true;
}

void Kernel::PollEvents()
{
	glfwPollEvents();
}

} //end of namespace JHL
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
