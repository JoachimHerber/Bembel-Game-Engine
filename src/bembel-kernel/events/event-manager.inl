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
inline void EventManager::Broadcast(EventType& event)
{
	auto channel = GetChannel<EventType>();
	if(channel)
		channel->Broadcast(event);
}

template <typename EventType, typename EventHandlerType>
inline bool EventManager::RemoveHandler(EventHandlerType* handler)
{
	auto channel = GetChannel<EventType>();
	if(!channel)
		return false;
	return channel->RemoveHandler(handler);
}

template <typename EventType, typename EventHandlerType>
inline bool EventManager::AddHandler(EventHandlerType* handler)
{
	auto channel = GetChannel<EventType>();
	if(!channel)
		return false;
	return channel->AddHandler(handler);
}

template <typename EventType>
inline EventChannel<EventType>* EventManager::GetChannel()
{
	auto& channel = channels_[typeid(EventType)];

	if( !channel )
		channel = std::make_unique<EventChannel<EventType>>();

	return static_cast<EventChannel<EventType>*>(channel.get());
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/