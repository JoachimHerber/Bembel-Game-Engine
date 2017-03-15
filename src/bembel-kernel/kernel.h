#ifndef BEMBEL_KERNEL_KERNEL_H_
#define BEMBEL_KERNEL_KERNEL_H_
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "bembel-config.h"

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

#include <bembel-base/logging/logger.h>

/*============================================================================*/
/* FORWARD DECLARATIONS                                                       */
/*============================================================================*/
namespace bembel{

class EventManager;
class AssetManager;
class DisplayManager;
class System;

} //end of namespace bembel
/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel{

class BEMBEL_API Kernel final
{
public:
	Kernel();
	~Kernel();
	
	EventManager*   GetEventManager() const;
	AssetManager*   GetAssetManager() const;
	DisplayManager* GetDisplayManager() const;

	template<typename SystemType, typename ... Args>
	SystemType* AddSystem( Args&& ... args );
	bool RemoveSystem(const std::string& name);
	System* GetSystem(const std::string& name);


	bool LoadSetting(const std::string& configFileName);

	bool InitSystems();
	void UpdateSystems(double timeSinceLastUpdate);
	void ShutdownSystems();

	void PollEvents();
private:
	std::unique_ptr<EventManager>   _eventMgr;
	std::unique_ptr<AssetManager>   _assetMgr;
	std::unique_ptr<DisplayManager> _displayMgr;

	std::unordered_map<std::string, unsigned> _systemMapping;
	std::vector<std::unique_ptr<System>>      _systems;
};

} //end of namespace bembel
/*============================================================================*/
/* TEMPLATE IMPLEMENTATION                                                    */
/*============================================================================*/
namespace bembel {

template<typename SystemType, typename ... Args>
inline SystemType* Kernel::AddSystem( Args&& ... args )
{
	auto system = std::make_unique<SystemType>(this, args...);
	auto system_pointer = system.get();

	const std::string& name = system->GetName();

	auto it = _systemMapping.find( name );
	if( it != _systemMapping.end() )
	{
		BEMBEL_LOG_ERROR()
			<< "System \""
			<< system->GetName()
			<< "\" has already been added."
			<< std::endl;
		return nullptr;
	}

	for( size_t n = 0; n <_systems.size(); ++n )
	{
		if( !_systems[n] )
		{
			_systems[n] = std::move(system);
			_systemMapping.emplace( name, n );
			return system_pointer;
		}
	}

	_systemMapping.emplace( name, _systems.size() );
	_systems.push_back( std::move( system ));
	return system_pointer;
}

} //end of namespace bembe
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
