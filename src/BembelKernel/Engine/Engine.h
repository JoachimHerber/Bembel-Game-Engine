#ifndef BEMBEL_ENGINE_H
#define BEMBEL_ENGINE_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include <BembelConfig.h>
#include <BembelBase/XML.h>

#include <memory>
#include <string>
#include <vector>
#include <map>

/*============================================================================*/
/* FORWARD DECLARATIONS                                                       */
/*============================================================================*/
namespace bembel{

class EventManager;
class DisplayManager;
class System;

} //end of namespace bembel
/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel{

class BEMBEL_API Engine
{
public:
	Engine();
	~Engine();

	bool InitSystems();
	bool InitSystems(const xml::Element* );
	void ShutdownSystems();

	void UpdateSystems(double timeSinceLastUpdate);

	bool AddSystem(std::shared_ptr<System>);
	bool RemoveSystem( const std::string& name );

	std::shared_ptr<System> GetSystem(const std::string& name);

private:
	std::map<std::string, unsigned> _systemMapping;
	std::vector<std::shared_ptr<System>> _systems;
};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
