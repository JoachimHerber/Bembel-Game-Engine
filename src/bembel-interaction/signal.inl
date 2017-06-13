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

/*============================================================================*/
/* INLINE IMPLEMENTETION                                                      */
/*============================================================================*/
namespace bembel {

template<typename ... TArgs>
inline Signal<TArgs ...>::Signal()
{}

template<typename ... TArgs>
inline Signal<TArgs ...>::~Signal()
{}

template<typename ... TArgs>
inline void Signal<TArgs ...>::AddSlot(void(*function)(TArgs...))
{
	AddSlot(Slot<TArgs...>(function));
}

template<typename ... TArgs>
template<class C>
inline void Signal<TArgs ...>::AddSlot(
	C*  objekt, void(C::* method)(TArgs...))
{
	AddSlot(Slot<TArgs...>(objekt, method));
}

template<typename ... TArgs>
template<class C>
inline void Signal<TArgs ...>::AddSlot(
	C* objekt, void(C::* method)(TArgs...) const)
{
	AddSlot(Slot<TArgs...>(objekt, method));
}

template<typename ... TArgs>
template<class C>
inline void Signal<TArgs ...>::AddSlot(C* functor)
{
	AddSlot(Slot<TArgs...>(functor));
}

template<typename ... TArgs>
inline void Signal<TArgs ...>::RemoveSlot(
	void(*function)(TArgs...))
{
	RemoveSlot(Slot<TArgs...>(function));
}

template<typename ... TArgs>
template<class C>
inline void Signal<TArgs ...>::RemoveSlot(
	C* objekt, void(C::* method)(TArgs...))
{
	RemoveSlot(Slot<TArgs...>(objekt, method));
}

template<typename ... TArgs>
template<class C>
inline void Signal<TArgs ...>::RemoveSlot(
	C* objekt, void(C::* method)(TArgs...) const)
{
	RemoveSlot(Slot<TArgs...>(objekt, method));
}

template<typename ... TArgs>
template<class C>
inline void Signal<TArgs ...>::RemoveSlot(C* functor)
{
	RemoveSlot(Slot<TArgs...>(functor));
}

template<typename ... TArgs>
inline void Signal<TArgs ...>::AddSlot(Slot<TArgs...>&& slot)
{
	std::lock_guard<std::mutex> lock(_mutex);
	_slots.push_back(slot);
}

template<typename ... TArgs>
inline void Signal<TArgs ...>::RemoveSlot(Slot<TArgs...>&& slot)
{
	std::lock_guard<std::mutex> lock(_mutex);

	auto it = std::find(_slots.begin(), _slots.end(), slot);

	if (it != _slots.end())
		_slots.erase(it);
}

template<typename ... TArgs>
void Signal<TArgs ...>::Emit(TArgs ... oArgs)
{
	SlotVector slots;
	{
		std::lock_guard<std::mutex> lock(_mutex);
		slots = _slots;
	}

	for(auto it : slots)
		it(oArgs...);
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/