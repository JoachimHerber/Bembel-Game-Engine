#ifndef BEMBEL_KERNEL_APPLICATION_H_
#define BEMBEL_KERNEL_APPLICATION_H_
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "bembel-config.h"

#include <memory>

/*============================================================================*/
/* FORWARD DECLARATIONS                                                       */
/*============================================================================*/
namespace bembel{

class Kernel;

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
	std::unique_ptr<Kernel> _kernel;

private:
	bool _quite;
};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
