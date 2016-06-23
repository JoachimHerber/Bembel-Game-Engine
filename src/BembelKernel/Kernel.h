#ifndef BEMBEL_KERNEL_H
#define BEMBEL_KERNEL_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include <BembelConfig.h>

#include <memory>
#include <string>

/*============================================================================*/
/* FORWARD DECLARATIONS                                                       */
/*============================================================================*/
namespace bembel{

class EventManager;
class DisplayManager;
class Engine;

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
	
	std::shared_ptr<EventManager>   GetEventManager() const;
	std::shared_ptr<DisplayManager> GetDisplayManager() const;

	Engine* GetEngine() const;

	bool LoadSetting(const std::string& configFileName);

	void PollEvents();

private:
	std::shared_ptr<EventManager>   _eventMgr;
	std::shared_ptr<DisplayManager> _displayMgr;

	std::unique_ptr<Engine> _engine;

};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
