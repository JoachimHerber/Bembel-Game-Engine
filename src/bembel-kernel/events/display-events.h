#ifndef BEMBEL_KERNEL_EVENTS_DISPLAYEVENTS_H_
#define BEMBEL_KERNEL_EVENTS_DISPLAYEVENTS_H_
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "bembel-config.h"
#include "event-manager.h"

#include <glm/glm.hpp>

/*============================================================================*/
/* FORWARD DECLARATIONS                                                       */
/*============================================================================*/
namespace bembel {

class Window;

} //end of namespace bembel
/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel {

struct BEMBEL_API WindowOpendEvent
{
	Window* const window;
};
struct BEMBEL_API WindowClosedEvent
{
	Window* const window;
};
struct BEMBEL_API WindowUpdateEvent
{
	Window* const window;
};


struct BEMBEL_API WindowMovedEvent
{
	Window* const window;
	const glm::vec2 position;
};
struct BEMBEL_API WindowResizeEvent
{
	Window* const window;
	const glm::vec2 size;
};

struct BEMBEL_API WindowShouldCloseEvent
{
	Window* const window;
};


struct BEMBEL_API WindowGainedFocusEvent
{
	Window* const window;
};
struct BEMBEL_API WindowLostFocusEvent
{
	Window* const window;
};


struct BEMBEL_API WindowIconifyedEvent
{
	Window* const window;
};
struct BEMBEL_API WindowRestoredEvent
{
	Window* const window;
};


struct BEMBEL_API FrameBufferResizeEvent
{
	Window* const   window;
	const glm::vec2 size;
};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
