#ifndef BEMBEL_INPUTEVENTS_H
#define BEMBEL_INPUTEVENTS_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include <BembelConfig.h>
#include "EventManager.h"

#include <glm/glm.hpp>

/*============================================================================*/
/* FORWARD DECLARATIONS                                                       */
/*============================================================================*/
namespace bembel{

class Window;

} //end of namespace bembel
/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel {


struct BEMBEL_API KeyPressEvent
{
	Window* const window;
	int keyID;
	int scancode;
	int mods;
	BEMBEL_EVENT_INTERVACE_DECLADRATION
};
struct BEMBEL_API KeyRepeatEvent
{
	Window* const window;
	int keyID;
	int scancode;
	int mods;
	BEMBEL_EVENT_INTERVACE_DECLADRATION
};
struct BEMBEL_API KeyReleaseEvent
{
	Window* const window;
	int keyID;
	int scancode;
	int mods;
	BEMBEL_EVENT_INTERVACE_DECLADRATION
};

struct BEMBEL_API TextInputEvent
{
	Window* const window;
	char32_t      character;
	BEMBEL_EVENT_INTERVACE_DECLADRATION
};

struct BEMBEL_API MouseButtonPressEvent
{
	Window* const window;
	int buttonID;
	int mods;
	BEMBEL_EVENT_INTERVACE_DECLADRATION
};
struct BEMBEL_API MouseButtonRepeatEvent
{
	Window* const window;
	int buttonID;
	int mods;
	BEMBEL_EVENT_INTERVACE_DECLADRATION
};
struct BEMBEL_API MouseButtonReleaseEvent
{
	Window* const window;
	int buttonID;
	int mods;
	BEMBEL_EVENT_INTERVACE_DECLADRATION
};

struct BEMBEL_API CursorMovedEvent
{
	Window* const window;
	glm::dvec2 position;
	BEMBEL_EVENT_INTERVACE_DECLADRATION
};

struct BEMBEL_API CursorEnteredEvent
{
	Window* const window;
	BEMBEL_EVENT_INTERVACE_DECLADRATION
};
struct BEMBEL_API CursorLeftEvent
{
	Window* const window;
	BEMBEL_EVENT_INTERVACE_DECLADRATION
};

struct BEMBEL_API ScrollEvent
{
	Window* const window;
	double x;
	double y;
	BEMBEL_EVENT_INTERVACE_DECLADRATION
};

struct BEMBEL_API FileDropEvent
{
	Window* window;

	std::vector<std::string> files;

	BEMBEL_EVENT_INTERVACE_DECLADRATION
};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
