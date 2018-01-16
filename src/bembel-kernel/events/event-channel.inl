/******************************************************************************/
/* ************************************************************************** */
/* *                                                                        * */
/* *    MIT License                                                         * */
/* *                                                                        * */
/* *   Copyright(c) 2018 Joachim Herber                                     * */
/* *                                                                        * */
/* *   Permission is hereby granted, free of charge, to any person          * */
/* *   obtaining copy of this software and associated documentation files   * */
/* *   (the "Software"), to deal in the Software without restriction,       * */
/* *   including without limitation the rights to use, copy, modify, merge, * */
/* *   publish, distribute, sublicense, and/or sell copies of the Software, * */
/* *   and to permit persons to whom the Software is furnished to do so,    * */
/* *   subject to the following conditions :                                * */
/* *                                                                        * */
/* *   The above copyright notice and this permission notice shall be       * */
/* *   included in all copies or substantial portions of the Software.      * */
/* *                                                                        * */
/* *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,      * */
/* *   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF   * */
/* *   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND                * */
/* *   NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS   * */
/* *   BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN   * */
/* *   ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN    * */
/* *   CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE     * */
/* *   SOFTWARE.                                                            * */
/* *                                                                        * */
/* ************************************************************************** */
/******************************************************************************/

/*============================================================================*/
/* INLINE IMPLEMENTETION                                                      */
/*============================================================================*/
namespace bembel {

template <typename EventType>
template <typename EventHandlerType>
inline bool EventChannel<EventType>::AddHandler(EventHandlerType* handler)
{
	std::lock_guard<std::mutex> lock(mutex_);

	for (auto it : event_handler_)
	{
		if (it.event_handler == handler)
			return false; // handler already registered
	}

	HandlerAdapter adapter;
	adapter.event_handler = handler;
	adapter.forward_event_function = ForwardEvent<EventHandlerType>;
	event_handler_.push_back(adapter);
	return true;
}

template <typename EventType>
template <typename EventHandlerType>
inline bool EventChannel<EventType>::RemoveHandler(EventHandlerType* handler)
{
	std::lock_guard<std::mutex> lock(mutex_);

	for(auto it = event_handler_.begin(); it <= event_handler_.end(); ++it)
	{
		if(it->event_handler == handler)
		{
			event_handler_.erase(it);
			return true;
		}
	}

	return false; // handler not found
}

template <typename EventType>
inline void EventChannel<EventType>::Broadcast(EventType& event)
{
	std::vector<HandlerAdapter> handler;
	{
		std::lock_guard<std::mutex> lock(mutex_);
		handler = event_handler_;
	}

	for(auto it : handler)
		it.forward_event_function(it.event_handler, event);
}

template <typename EventType>
template <typename EventHandlerType>
inline void EventChannel<EventType>::ForwardEvent(
	void* handler, EventType& event)
{
	static_cast<EventHandlerType*>(handler)->HandleEvent(event);
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/