/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "interaction-system.h"
#include "input/mouse.h"
#include "input/keyboard.h"

#include <bembel-base/xml.h>
#include <bembel-base/logging/logger.h>
#include <bembel-kernel/kernel.h>

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel {

InteractionSystem::InteractionSystem(Kernel* kernel)
	: System(kernel, "Interaction")
{
	_mouse    = std::make_unique<Mouse>(kernel->GetEventManager());
	_keyboard = std::make_unique<Keyboard>(kernel->GetEventManager());
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
