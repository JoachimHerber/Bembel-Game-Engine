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

	bool AddSystem(std::shared_ptr<System>);
	bool RemoveSystem(const std::string& name);
	std::shared_ptr<System> GetSystem(const std::string& name);


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
	std::vector<std::shared_ptr<System>>      _systems;
};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
