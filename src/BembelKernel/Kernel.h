#ifndef BEMBEL_KERNEL_H
#define BEMBEL_KERNEL_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include <BembelConfig.h>
#include <memory>

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
	
	EventManager*   GetEventManager() const;
	DisplayManager* GetDisplayManager() const;
	Engine*         GetEngine() const;
	
	void PollEvents();

private:
	std::unique_ptr<EventManager>   _eventMgr;
	std::unique_ptr<DisplayManager> _displayMgr;

	std::unique_ptr<Engine> _engine;

};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
