/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "Events.h"

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel{

BEMBEL_EVENT_INTERVACE_IMPLEMENTATION(WindowOpendEvent);
BEMBEL_EVENT_INTERVACE_IMPLEMENTATION(WindowClosedEvent);
BEMBEL_EVENT_INTERVACE_IMPLEMENTATION(WindowUpdateEvent);

BEMBEL_EVENT_INTERVACE_IMPLEMENTATION(WindowMovedEvent);
BEMBEL_EVENT_INTERVACE_IMPLEMENTATION(WindowResizeEvent);

BEMBEL_EVENT_INTERVACE_IMPLEMENTATION(WindowShouldCloseEvent);

BEMBEL_EVENT_INTERVACE_IMPLEMENTATION(WindowGainedFocusEvent);
BEMBEL_EVENT_INTERVACE_IMPLEMENTATION(WindowLostFocusEvent);

BEMBEL_EVENT_INTERVACE_IMPLEMENTATION(WindowIconifyedEvent);
BEMBEL_EVENT_INTERVACE_IMPLEMENTATION(WindowRestoredEvent);

BEMBEL_EVENT_INTERVACE_IMPLEMENTATION(FrameBufferResizeEvent);

BEMBEL_EVENT_INTERVACE_IMPLEMENTATION(KeyPressEvent);
BEMBEL_EVENT_INTERVACE_IMPLEMENTATION(KeyRepeatEvent);
BEMBEL_EVENT_INTERVACE_IMPLEMENTATION(KeyReleaseEvent);

BEMBEL_EVENT_INTERVACE_IMPLEMENTATION(TextInputEvent);

BEMBEL_EVENT_INTERVACE_IMPLEMENTATION(MouseButtonPressEvent);
BEMBEL_EVENT_INTERVACE_IMPLEMENTATION(MouseButtonRepeatEvent);
BEMBEL_EVENT_INTERVACE_IMPLEMENTATION(MouseButtonReleaseEvent);

BEMBEL_EVENT_INTERVACE_IMPLEMENTATION(CursorMovedEvent);
BEMBEL_EVENT_INTERVACE_IMPLEMENTATION(CursorEnteredEvent);
BEMBEL_EVENT_INTERVACE_IMPLEMENTATION(CursorLeftEvent);

BEMBEL_EVENT_INTERVACE_IMPLEMENTATION(ScrollEvent);

BEMBEL_EVENT_INTERVACE_IMPLEMENTATION(FileDropEvent);

} //end of namespace bembel
  /*============================================================================*/
  /* END OF FILE                                                                */
  /*============================================================================*/
