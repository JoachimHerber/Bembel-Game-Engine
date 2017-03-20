#ifndef BEMBEL_KERNEL_EVENTS_INPUTEVENTS_H_
#define BEMBEL_KERNEL_EVENTS_INPUTEVENTS_H_
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


struct BEMBEL_API KeyPressEvent
{
	Window* const window;
	int key_id;
	int scancode;
	int mods;
};
struct BEMBEL_API KeyRepeatEvent
{
	Window* const window;
	int key_id;
	int scancode;
	int mods;
};
struct BEMBEL_API KeyReleaseEvent
{
	Window* const window;
	int key_id;
	int scancode;
	int mods;
};

struct BEMBEL_API TextInputEvent
{
	Window* const window;
	char32_t      character;
};

struct BEMBEL_API MouseButtonPressEvent
{
	Window* const window;
	int button_id;
	int mods;
};
struct BEMBEL_API MouseButtonRepeatEvent
{
	Window* const window;
	int button_id;
	int mods;
};
struct BEMBEL_API MouseButtonReleaseEvent
{
	Window* const window;
	int button_id;
	int mods;
};

struct BEMBEL_API CursorMovedEvent
{
	Window* const window;
	glm::dvec2 position;
};

struct BEMBEL_API CursorEnteredEvent
{
	Window* const window;
};
struct BEMBEL_API CursorLeftEvent
{
	Window* const window;
};

struct BEMBEL_API ScrollEvent
{
	Window* const window;
	double x;
	double y;
};

struct BEMBEL_API FileDropEvent
{
	Window* window;

	std::vector<std::string> files;
};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
