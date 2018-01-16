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

#ifndef BEMBEL_KERNEL_EVENTS_EVENTMANAGER_H_
#define BEMBEL_KERNEL_EVENTS_EVENTMANAGER_H_
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "bembel-config.h"
#include "event-channel.hpp"

#include <vector>
#include <memory>
#include <unordered_map>
#include <typeindex>
#include <typeinfo>

/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel {

class BEMBEL_API EventManager final
{
public:
	EventManager();
	EventManager(const EventManager&) = delete;
	EventManager& operator=(const EventManager&) = delete;
	~EventManager();

	template <typename EventType, typename EventHandlerType>
	bool AddHandler(EventHandlerType* handler);
	template <typename EventType, typename EventHandlerType>
	bool RemoveHandler(EventHandlerType* handler);

	template <typename EventType>
	void Broadcast(EventType& event);

	template <typename EventType>
	EventChannel<EventType>* GetChannel();

private:
	using EventChannelPointer = std::unique_ptr<EventChannelBase>;
	std::unordered_map<std::type_index, EventChannelPointer> channels_;
};

} //end of namespace bembel
/*============================================================================*/
/* INLINE IMPLEMENTATIONS													  */
/*============================================================================*/

#include "event-manager.inl"

/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
