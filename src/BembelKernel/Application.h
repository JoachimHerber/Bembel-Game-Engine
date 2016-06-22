#ifndef BEMBEL_APPLICATION_H
#define BEMBEL_APPLICATION_H
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
class Engine;

} //end of namespace bembel
/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel{

class BEMBEL_API Application
{
public:
	Application();
	virtual ~Application();
	
	bool Run();
	void Quit();

protected:
	virtual void MainLoop();

	virtual bool Init()    = 0;
	virtual void Cleanup() = 0;

	virtual void Update(double timeDelta) = 0;

protected:
	std::shared_ptr<EventManager> _eventMgr;
	std::unique_ptr<Engine> _engine;

private:
	bool _quite;
};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
