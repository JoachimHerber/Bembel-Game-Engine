/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "kernel.h"
#include "system.h"
#include "events/event-manager.h"
#include "assets/asset-manager.h"
#include "display/display-manager.h"

#include <chrono>

#include <glfw/glfw3.h>

#include <bembel-base/xml.h>
#include <bembel-base/logging/logger.h>

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel{

Kernel::Kernel()
	: event_manager_(std::make_unique<EventManager>())
	, asset_manager_(std::make_unique<AssetManager>())
{
	if( glfwInit() == GL_FALSE )
	{
		BEMBEL_LOG_ERROR() << "Failed to initialize GLFW" << std::endl;
		throw std::exception();
	}

	display_manager_ = std::make_unique<DisplayManager>(this);
}

Kernel::~Kernel()
{
	display_manager_.reset();

	glfwTerminate();
}

EventManager* Kernel::GetEventManager() const
{
	return event_manager_.get();
}

AssetManager* Kernel::GetAssetManager() const
{
	return asset_manager_.get();
}

DisplayManager* Kernel::GetDisplayManager() const
{
	return display_manager_.get();
}

bool Kernel::RemoveSystem(const std::string& name)
{
	auto it = system_mapping_.find(name);
	if (it == system_mapping_.end())
	{
		BEMBEL_LOG_WARNING() << "Unknown system '" << name << "'." << std::endl;
		return false;
	}

	systems_[it->second].reset();
	system_mapping_.erase(it);
	return true;
}

System* Kernel::GetSystem(const std::string& name)
{
	auto it = system_mapping_.find(name);
	if (it != system_mapping_.end())
		return systems_[it->second].get();

	BEMBEL_LOG_WARNING() << "Unknown system '" << name << "'." << std::endl;
	return nullptr;
}

bool Kernel::InitSystems()
{
	for (auto& system : systems_)
	{
		if (!system->Init())
			return false;
	}
	return true;
}

void Kernel::UpdateSystems(double time_since_last_update)
{
	for (auto& system : systems_)
		system->Update(time_since_last_update);
}
void Kernel::ShutdownSystems()
{
	for (auto& system : systems_)
		system->Shutdown();
}
bool Kernel::LoadSetting(const std::string& config_file_name)
{
	xml::Document doc;
	if (doc.LoadFile(config_file_name.c_str()) != tinyxml2::XML_SUCCESS)
	{
		BEMBEL_LOG_ERROR()
			<< "Failed to lode file '" << config_file_name << "'\n"
			<< doc.ErrorName() << std::endl;
		return false;
	}

	const xml::Element* root = doc.FirstChildElement("Bembel");
	if (!root)
		return false;

	const xml::Element* display = root->FirstChildElement("Display");
	if (display)
		display_manager_->CreateWindows(display);

	const xml::Element* systems = root->FirstChildElement("Systems");
	if (systems)
	{
		for (auto& system : systems_)
		{
			const xml::Element* system_config =
				systems->FirstChildElement(system->GetName().c_str());

			if (!system->Configure(system_config))
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
