/******************************************************************************/
/* ************************************************************************** */
/* *                                                                        * */
/* *    MIT License                                                         * */
/* *                                                                        * */
/* *   Copyright(c) 2017 Joachim Herber                                     * */
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

#ifndef BEMBEL_INTERACTION_SIGNAL_H_
#define BEMBEL_INTERACTION_SIGNAL_H_
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "slot.hpp"

#include <mutex>
#include <vector>

/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel {

template<typename ... TArgs>
class Signal
{
public:
	Signal();
	Signal(const Signal& other) = delete;
	~Signal();

	void AddSlot(void(*function)(TArgs...));
	template<class C>
	void AddSlot(C* object, void(C::* method)(TArgs...));
	template<class C>
	void AddSlot(C* object, void(C::* method)(TArgs...) const);
	template<class C>
	void AddSlot(C* functor);

	void AddSlot(Slot<TArgs...>&& slot);

	void RemoveSlot(void(*function)(TArgs...));
	template<class C>
	void RemoveSlot(C* object, void(C::* method)(TArgs...));
	template<class C>
	void RemoveSlot(C* object, void(C::* method)(TArgs...) const);
	template<class C>
	void RemoveSlot(C* functor);

	void RemoveSlot(Slot<TArgs...>&& slot);

	void Emit(TArgs ... args);

private:
	using SlotVector = std::vector<Slot<TArgs...>>;

	SlotVector _slots;

	std::mutex _mutex;
};

} //end of namespace bembel
/*============================================================================*/
/* INLINE IMPLEMENTATIONS													  */
/*============================================================================*/

#include "Signal.inl"

/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
