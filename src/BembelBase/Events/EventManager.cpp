/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "EventManager.h"

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel {

unsigned EventManager::_nextUnusedEventTypeID = 0;

EventManager::EventManager()
{}

EventManager::~EventManager()
{
	for(EventChannelBase* channel : _channels)
		delete channel;
}

unsigned EventManager::GetNextFreeEventTypeID()
{
	return _nextUnusedEventTypeID++;
}
} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/


