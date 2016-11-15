/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "InteractionSystem.h"

#include "Input/Mouse.h"
#include "Input/Keyboard.h"

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
	_mouse    = std::make_unique<Mouse>(kernel->GetEventManager().get());
	_keyboard = std::make_unique<Keyboard>(kernel->GetEventManager().get());
}
InteractionSystem::~InteractionSystem()
{
}

bool InteractionSystem::Configure(const xml::Element* properties)
{
	if (!properties)
		return true;

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

Mouse* InteractionSystem::GetMouse() const
{
	return _mouse.get();
}

Keyboard* InteractionSystem::GetKeyboard() const
{
	return _keyboard.get();
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
