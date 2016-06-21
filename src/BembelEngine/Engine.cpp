/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "Engine.h"
#include "System.h"

#include <BembelBase/XML.h>
#include <BembelBase/Logging/Logger.h>
#include <BembelBase/Events/EventManager.h>

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel{

Engine::Engine()
{}

Engine::~Engine()
{}

bool Engine::InitSystems()
{
	for (auto system : _systems)
	{
		if (!system->Init())
			return false;
	}
	return true;
}

bool Engine::InitSystems(const std::string& configFileName)
{
	xml::Document doc;
	if (doc.LoadFile(configFileName.c_str()) != tinyxml2::XML_NO_ERROR)
	{
		BEMBEL_LOG_ERROR()
			<< "Failed to lode file '" << configFileName << "'\n"
			<< doc.ErrorName() << std::endl;
		return false;
	}

	const xml::Element* root = doc.FirstChildElement("Engine");
	if (!root)
		return false;

	for (auto system : _systems)
	{
		const xml::Element* config = 
			root->FirstChildElement(system->GetName().c_str());

		if (!system->Init(config))
			return false;
	}
	return true;
}

void Engine::ShutdownSystems()
{
	// shut down systems in refers order
	for (size_t n = _systems.size(); n > 0; --n)
		_systems[n-1]->Shutdown();
}

void Engine::UpdateSystems(double timeSinceLastUpdate)
{
	for (auto system : _systems)
		system->Update(timeSinceLastUpdate);
}

bool Engine::AddSystem(std::shared_ptr<System> system)
{
	const std::string& name = system->GetName();

	auto it = _systemMapping.find(name);
	if(it != _systemMapping.end())
	{
		BEMBEL_LOG_ERROR() 
			<< "System \"" 
			<< system->GetName() 
			<< "\" has already been added." 
			<< std::endl;
		return false;
	}

	for (size_t n=0; n <_systems.size(); ++n)
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

bool Engine::RemoveSystem(const std::string& name)
{
	auto it = _systemMapping.find(name);
	if(it == _systemMapping.end())
	{
		BEMBEL_LOG_WARNING() << "Unknown system '" << name << "'." << std::endl;
		return false;
	}

	_systems[it->second].reset();
	_systemMapping.erase(it);

	return true;
}

std::shared_ptr<System> Engine::GetSystem(const std::string& name)
{
	auto it = _systemMapping.find(name);
	if(it != _systemMapping.end())
		return _systems[it->second];

	BEMBEL_LOG_WARNING() << "Unknown system '" << name << "'." << std::endl;
	return nullptr;
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
