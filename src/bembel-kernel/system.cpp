/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "system.h"

#include <bembel-base/logging/logger.h>

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel{

System::System(Kernel* kernel, const std::string& name)
	: kernel_(kernel)
	, system_name_(name)
{}

System::~System()
{}

const std::string& System::GetName() const
{
	return system_name_;
}
Kernel* System::GetKernel() const
{
	return kernel_;
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
