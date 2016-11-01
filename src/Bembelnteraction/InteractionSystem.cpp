/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "InteractionSystem.h"

#include <BembelBase/XML.h>
#include <BembelBase/Logging/Logger.h>
#include <BembelKernel/Kernel.h>

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel {

InteractionSystem::InteractionSystem(Kernel* kernel)
	: System(kernel, "Interaction")
{
}
InteractionSystem::~InteractionSystem()
{
}

bool InteractionSystem::Configure(const xml::Element* properties)
{
	if (!properties)
		return false;

	return true;
}

bool InteractionSystem::Init()
{
	return true;
}

void InteractionSystem::Shutdown()
{
}

void InteractionSystem::Update(double)
{
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
