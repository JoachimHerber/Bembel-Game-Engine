/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "DisplayEvents.h"

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

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
