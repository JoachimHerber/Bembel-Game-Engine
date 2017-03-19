#ifndef BEMBE_KERNEL_SYSTEM_H_
#define BEMBE_KERNEL_SYSTEM_H_
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "bembel-config.h"

#include <memory>

#include <bembel-base/xml.h>

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

class BEMBEL_API System
{
public:
	System(Kernel* kernel, const std::string& name);
	virtual ~System();
	
	const std::string& GetName() const;
	Kernel*  GetKernel() const;

	virtual bool Configure(const xml::Element*);

	virtual bool Init();
	virtual void Shutdown();

	virtual void Update(double time_since_last_update) = 0;

protected:
	Kernel*           kernel_;
	const std::string system_name_;
};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
