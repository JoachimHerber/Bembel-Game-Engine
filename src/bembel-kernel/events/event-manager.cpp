/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "event-manager.h"

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel {

unsigned EventManager::next_unused_event_type_id_ = 0;

EventManager::EventManager()
{}

EventManager::~EventManager()
{
	for( EventChannelBase* channel : channels_ )
		delete channel;
}

unsigned EventManager::GetNextFreeEventTypeID()
{
	return next_unused_event_type_id_++;
}
} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/


