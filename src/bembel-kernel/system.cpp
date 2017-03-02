/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "System.h"
#include <BembelBase/Logging/Logger.h>

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel{

System::System(Kernel* kernel, const std::string& name)
	: _kernel(kernel)
	, _systemName(name)
{}

System::~System()
{}

const std::string& System::GetName() const
{
	return _systemName;
}
Kernel* System::GetKernel() const
{
	return _kernel;
}

bool System::Configure(const xml::Element*)
{
	return true;
}

bool System::Init()
{
	return true;
}

void System::Shutdown()
{
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
