#ifndef BEMBE_SYSTEM_H
#define BEMBE_SYSTEM_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include <BembelConfig.h>

#include <BembelBase/XML.h>

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

	virtual void Update(double timeSinceLastUpdate) = 0;

protected:
	Kernel*           _kernel;
	const std::string _systemName;
};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
